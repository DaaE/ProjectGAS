// Fill out your copyright notice in the Description page of Project Settings.


#include "SayuInventoryWidget.h"

#include "SayuInventoryItemWidget.h"
#include "SayuInventorySlotWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/UniformGridPanel.h"
#include "Inventory/SayuInventoryComponent.h"
#include "Items/SayuItemDefinition.h"
#include "Items/SayuItemInstance.h"

void USayuInventoryWidget::SetInventoryComponent(USayuInventoryComponent* InInventoryComponent)
{
	InventoryComponent = InInventoryComponent;
	BuildGrid();
	RefreshItems();
}

void USayuInventoryWidget::BuildGrid()
{
	if (!GridPanel || !InventoryComponent || !SlotWidgetClass)
	{
		return;
	}

	GridPanel->ClearChildren();
	GridPanel->SetMinDesiredSlotWidth(CellSize);
	GridPanel->SetMinDesiredSlotHeight(CellSize);

	const int32 Width = InventoryComponent->GetGridWidth();
	const int32 Height = InventoryComponent->GetGridHeight();
	SlotWidgets.SetNum(Width * Height);

	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			USayuInventorySlotWidget* NewSlot = CreateWidget<USayuInventorySlotWidget>(this, SlotWidgetClass);
			if (!NewSlot)
			{
				continue;
			}

			GridPanel->AddChildToUniformGrid(NewSlot, Y, X);
			SlotWidgets[Y * Width + X] = NewSlot;
		}
	}
}

void USayuInventoryWidget::RefreshItems()
{
	if (!ItemCanvas || !InventoryComponent || !ItemWidgetClass)
	{
		return;
	}

	ItemCanvas->ClearChildren();

	for (const FSayuInventoryEntry& Entry : InventoryComponent->GetEntries())
	{
		if (!Entry.Instance || !Entry.Instance->ItemDefinition)
		{
			continue;
		}

		USayuInventoryItemWidget* NewItemWidget = CreateWidget<USayuInventoryItemWidget>(this, ItemWidgetClass);
		if (!NewItemWidget)
		{
			continue;
		}

		NewItemWidget->SetItemInstance(Entry.Instance);

		if (UCanvasPanelSlot* CanvasSlot = ItemCanvas->AddChildToCanvas(NewItemWidget))
		{
			const FIntPoint Size = Entry.Instance->ItemDefinition->GridSize;
			CanvasSlot->SetPosition(FVector2D(Entry.TopLeft.X * CellSize, Entry.TopLeft.Y * CellSize));
			CanvasSlot->SetSize(FVector2D(Size.X * CellSize, Size.Y * CellSize));
		}
	}
}