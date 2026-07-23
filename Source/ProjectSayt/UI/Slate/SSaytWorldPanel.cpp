// Copyright (c) 2026 Sungmin JI. All Rights Reserved.

#include "SSaytWorldPanel.h"
#include "GameFramework/PlayerController.h"
#include "Layout/ArrangedChildren.h"

void SSaytWorldPanel::FSlot::Construct(const FChildren& SlotOwner, FSlotArguments&& InArgs)
{
	TSlotBase<FSlot>::Construct(SlotOwner, MoveTemp(InArgs));
	WorldLocation = InArgs._WorldLocation;
}

SSaytWorldPanel::SSaytWorldPanel() : Children(this)
{
	SetCanTick(false);
	// 게임 화면을 덮는 오버레이가 클릭을 가로채면 안 됨 —
	// 패널 자신은 히트테스트 투명, 자식은 각자 판단
	SetVisibility(EVisibility::SelfHitTestInvisible);
}

void SSaytWorldPanel::Construct(const FArguments& InArgs, APlayerController* InPlayerController)
{
	PlayerController = InPlayerController;
}

SSaytWorldPanel::FScopedWidgetSlotArguments SSaytWorldPanel::AddSlot()
{
	return FScopedWidgetSlotArguments{ MakeUnique<FSlot>(), Children, INDEX_NONE };
}

int32 SSaytWorldPanel::RemoveSlot(const TSharedRef<SWidget>& SlotWidget)
{
	return Children.Remove(SlotWidget);
}

void SSaytWorldPanel::ClearChildren()
{
	Children.Empty();
}

FVector2D SSaytWorldPanel::ComputeDesiredSize(float) const
{
	// Measure 패스: "얼마나 크고 싶니?"에 대한 이 패널의 답은 "주장 없음".
	// 뷰포트 전체를 오버레이로 받는 전제라, 스스로 크기를 요구하지 않습니다.
	return FVector2D::ZeroVector;
}

void SSaytWorldPanel::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	// Arrange 패스: 부모가 받은 영역(AllottedGeometry) 안에서
	// 자식마다 위치+크기를 "선언"해 명단(ArrangedChildren)에 올립니다.
	// 명단에 안 올라간 자식은 이 프레임에 존재하지 않는 것과 같습니다(그리기/입력 모두).
	APlayerController* PC = PlayerController.Get();
	if (!PC)
	{
		return;
	}

	for (int32 Index = 0; Index < Children.Num(); ++Index)
	{
		const FSlot& CurSlot = Children[Index];
		const TSharedRef<SWidget>& Widget = CurSlot.GetWidget();

		if (!ArrangedChildren.Accepts(Widget->GetVisibility()))
		{
			continue;
		}

		// [2-6 의도적 순진함] 투영을 Arrange 안에서 직접 수행 — 갱신 전략은 2-7에서 재구성
		FVector2D PixelPos = FVector2D::ZeroVector;
		const bool bInFront = PC->ProjectWorldLocationToScreen(CurSlot.GetWorldLocation(), PixelPos, true);
		if (!bInFront)
		{
			continue; // 카메라 뒤 → 명단 제외 (2-5 문답 Q1의 '유령 체력바' 방지 분기)
		}

		// 2-5 예고분: 투영 결과(뷰포트 픽셀) → Slate 로컬 단위.
		// Geometry의 누적 레이아웃 변환에서 스케일을 조회 (.Scale 멤버는 legacy)
		const FVector2D LocalPos = PixelPos / AllottedGeometry.GetAccumulatedLayoutTransform().GetScale();

		// 바 가로 중앙 + 기준점 아래 정렬 (머리 위치가 바의 '발밑'이 되도록)
		const FVector2D ChildSize = Widget->GetDesiredSize();
		const FVector2D ChildOffset(LocalPos.X - ChildSize.X * 0.5f, LocalPos.Y - ChildSize.Y);

		// 자식 배치 = "이 크기로, 이 변환(평행이동)을 부여한다" — 개편 후 FGeometry의 본래 모델
		ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(Widget, ChildSize, FSlateLayoutTransform(ChildOffset)));
	}
}

FChildren* SSaytWorldPanel::GetChildren()
{
	return &Children;
}