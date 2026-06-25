// Fill out your copyright notice in the Description page of Project Settings.


#include "SayuInventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/SayuItemDefinition.h"
#include "Items/SayuItemInstance.h"


void USayuInventorySlotWidget::SetOccupant(USayuItemInstance* Instance)
{
	if (!Instance || !Instance->ItemDefinition)
	{
		ItemLabel->SetText(FText::GetEmpty());
		SetToolTipText(FText::GetEmpty());
		return;
	}

	const FText& ItemName = Instance->ItemDefinition->ItemName;

	// 스택 1개면 이름만, 2개 이상이면 "이름 x개수"로 표시.
	if (Instance->StackCount > 1)
	{
		ItemLabel->SetText(FText::Format(FText::FromString(TEXT("{0} x{1}")),
			ItemName, FText::AsNumber(Instance->StackCount)));
	}
	else
	{
		ItemLabel->SetText(ItemName);
	}

	// 툴팁 뼈대 — UMG 기본 제공 함수라 이름만 연결해주면 끝.
	SetToolTipText(ItemName);
}

void USayuInventorySlotWidget::SetHighlight(ESayuPlacementHighlight NewState)
{
	switch (NewState)
	{
	case ESayuPlacementHighlight::None:
		HighlightOverlay->SetVisibility(ESlateVisibility::Hidden);
		break;

	case ESayuPlacementHighlight::Valid:
		HighlightOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
		HighlightOverlay->SetColorAndOpacity(FLinearColor(0.f, 1.f, 0.f, 0.4f));
		break;

	case ESayuPlacementHighlight::Invalid:
		HighlightOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
		HighlightOverlay->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.4f));
		break;
	}
}