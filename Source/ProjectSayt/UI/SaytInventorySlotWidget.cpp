// Copyright (c) 2026 Sungmin JI. All Rights Reserved.


#include "SaytInventorySlotWidget.h"

#include "SaytInventoryItemWidget.h"
#include "SaytInventoryWidget.h"
#include "Components/Image.h"
#include "Inventory/SaytInventoryComponent.h"
#include "Items/SaytItemInstance.h"
#include "Items/SaytItemDragDropOperation.h"


void USaytInventorySlotWidget::SetHighlight(ESaytPlacementHighlight NewState) const
{
	switch (NewState)
	{
	case ESaytPlacementHighlight::None:
		HighlightOverlay->SetVisibility(ESlateVisibility::Hidden);
		break;

	case ESaytPlacementHighlight::Valid:
		HighlightOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
		HighlightOverlay->SetColorAndOpacity(FLinearColor(0.f, 1.f, 0.f, 0.4f));
		break;

	case ESaytPlacementHighlight::Invalid:
		HighlightOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
		HighlightOverlay->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.4f));
		break;
	}
}

void USaytInventorySlotWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	
	const USaytItemDragDropOperation* DragOp = Cast<USaytItemDragDropOperation>(InOperation);
	if (!DragOp || !DragOp->DraggedInstance || !OwningInventoryWidget)
	{
		return;
	}

	USaytInventoryComponent* InventoryComponent = OwningInventoryWidget->GetInventoryComponent();
	if (!InventoryComponent)
	{
		return;
	}

	// 의도적 단순화: 지금은 호버 중인 한 칸만 하이라이트. 풋프린트 전체 하이라이트는
	// 다듬기 단계 후보로 남겨둠.
	const bool bCanPlace = InventoryComponent->CanPlaceItemAt(DragOp->DraggedInstance->ItemDefinition, GridPosition, DragOp->DraggedInstance);
	SetHighlight(bCanPlace ? ESaytPlacementHighlight::Valid : ESaytPlacementHighlight::Invalid);
	
	if (USaytInventoryItemWidget* DragVisual = Cast<USaytInventoryItemWidget>(DragOp->DefaultDragVisual))
	{
		DragVisual->SetValidityTint(bCanPlace);
	}
}

void USaytInventorySlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	SetHighlight(ESaytPlacementHighlight::None);
	
	const USaytItemDragDropOperation* DragOp = Cast<USaytItemDragDropOperation>(InOperation);
	if (USaytInventoryItemWidget* DragVisual = DragOp ? Cast<USaytInventoryItemWidget>(DragOp->DefaultDragVisual) : nullptr)
	{
		DragVisual->ResetTint();
	}
}

bool USaytInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	SetHighlight(ESaytPlacementHighlight::None);

	USaytItemDragDropOperation* DragOp = Cast<USaytItemDragDropOperation>(InOperation);
	if (!DragOp || !DragOp->DraggedInstance || !OwningInventoryWidget)
	{
		return false;
	}

	OwningInventoryWidget->HandleItemDropped(DragOp->DraggedInstance, GridPosition, DragOp->OriginalTopLeft);
	return true;
}
