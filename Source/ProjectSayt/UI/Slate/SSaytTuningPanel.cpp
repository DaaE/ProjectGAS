// Copyright (c) 2026 Sungmin JI. All Rights Reserved.


#include "SSaytTuningPanel.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/CoreStyle.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

// ---------- SSaytDragTrack ----------

void SSaytDragTrack::Construct(const FArguments& InArgs)
{
	Value = InArgs._Value;
	OnValueChanged = InArgs._OnValueChanged;
	bIsDragging = false;

	ChildSlot
	[
		SNew(SBox)
		.HeightOverride(24.f)
		.WidthOverride(280.f)
		[
			SNew(SOverlay)
			// 트랙 배경
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
				.BorderBackgroundColor(FLinearColor(0.15f, 0.15f, 0.15f))
			]
			// 핸들 — Padding을 GetHandlePadding()에 묶어서, Magnitude가 바뀌면
			// 다음 Paint 때 다른 위치에서 그려지게 합니다. OnPaint 없이 Slot만으로 위치를 옮기는 겁니다.
			+ SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Fill)
			.Padding(TAttribute<FMargin>::CreateSP(this, &SSaytDragTrack::GetHandlePadding))
			[
				SNew(SBox)
				.WidthOverride(HandleSize)
				[
					SNew(SBorder)
					.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
					.BorderBackgroundColor(FLinearColor(0.9f, 0.6f, 0.1f))
				]
			]
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FMargin SSaytDragTrack::GetHandlePadding() const
{
	// GetCachedGeometry() — 이 위젯이 마지막으로 Paint될 때 계산된 자기 자신의 크기/위치입니다.
	const float TrackWidth = GetCachedGeometry().GetLocalSize().X;
	const float UsableWidth = FMath::Max(0.f, TrackWidth - HandleSize);
	const float CurrentValue = Value.Get(0.f);
	return FMargin(CurrentValue * UsableWidth, 0.f, 0.f, 0.f);
}

FReply SSaytDragTrack::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDragging = true;
		UpdateValueFromMouse(MyGeometry, MouseEvent);
		// CaptureMouse: 드래그 중 커서가 위젯 밖으로 나가도 OnMouseMove를 계속 받게 합니다.
		return FReply::Handled().CaptureMouse(SharedThis(this));
	}
	return FReply::Unhandled();
}

FReply SSaytDragTrack::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bIsDragging)
	{
		UpdateValueFromMouse(MyGeometry, MouseEvent);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply SSaytDragTrack::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bIsDragging)
	{
		bIsDragging = false;
		return FReply::Handled().ReleaseMouseCapture();
	}
	return FReply::Unhandled();
}

void SSaytDragTrack::UpdateValueFromMouse(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const FVector2D LocalPos = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	const float TrackWidth = MyGeometry.GetLocalSize().X;
	const float NewValue = TrackWidth > 0.f ? FMath::Clamp(LocalPos.X / TrackWidth, 0.f, 1.f) : 0.f;
	OnValueChanged.ExecuteIfBound(NewValue);
}

// ---------- SSaytTuningPanel ----------

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSaytTuningPanel::Construct(const FArguments& InArgs)
{
	ChildSlot
   [
	   SNew(SVerticalBox)
	   + SVerticalBox::Slot().AutoHeight().Padding(8.f)
	   [
		   SNew(SSaytDragTrack)
		   .Value(TAttribute<float>::CreateLambda([this]() { return Model.Magnitude; }))
		   .OnValueChanged(this, &SSaytTuningPanel::HandleDragValueChanged)
	   ]
	   + SVerticalBox::Slot().AutoHeight().Padding(8.f)
	   [
		   SAssignNew(ProgressBarWidget, SProgressBar)
		   .Percent(TAttribute<TOptional<float>>::CreateLambda([this]() { return TOptional<float>(Model.Magnitude); }))
	   ]
	   + SVerticalBox::Slot().AutoHeight().Padding(8.f)
	   [
		   SAssignNew(ValueTextWidget, STextBlock)
		   .Text(TAttribute<FText>::CreateLambda([this]()
		   {
			   return FText::FromString(FString::Printf(TEXT("Magnitude: %.2f"), Model.Magnitude));
		   }))
	   ]
   ];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSaytTuningPanel::HandleDragValueChanged(float NewValue)
{
	Model.Magnitude = NewValue;

	// 여기가 핵심입니다 — Model을 바꾼다고 화면이 저절로 안 바뀝니다.
	// TAttribute는 "물어보면 답해준다"일 뿐, 누가 다시 물어보게 만들진 않습니다.
	// Invalidate(Paint)가 "다음 Paint 때 너 다시 그려"라고 명시적으로 지시하는 부분입니다.
	if (ProgressBarWidget.IsValid())
	{
		ProgressBarWidget->Invalidate(EInvalidateWidgetReason::Paint);
	}
	if (ValueTextWidget.IsValid())
	{
		ValueTextWidget->Invalidate(EInvalidateWidgetReason::Paint);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
