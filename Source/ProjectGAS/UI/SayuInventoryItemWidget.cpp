// Fill out your copyright notice in the Description page of Project Settings.


#include "SayuInventoryItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/SayuItemDefinition.h"
#include "Items/SayuItemInstance.h"


void USayuInventoryItemWidget::SetItemInstance(USayuItemInstance* Instance)
{
	if (!Instance || !Instance->ItemDefinition)
	{
		return;
	}
	
	const USayuItemDefinition* ItemDef = Instance->ItemDefinition;
	
	// 임시 동기 로딩 — 다음 단계(비동기 로딩 정식 구현)에서 FStreamableManager 기반으로
	// 교체할 예정. 지금은 "레이아웃이 의도대로 동작하는지"만 빠르게 확인하는 목적.
	if (UTexture2D* LoadedIcon = ItemDef->Icon.LoadSynchronous())
	{
		IconImage->SetBrushFromTexture(LoadedIcon);
	}
	
	// 스택 1개면 숫자 숨기고, 2개 이상이면 표시.
	if (Instance->StackCount > 1)
	{
		StackText->SetText(FText::AsNumber(Instance->StackCount));
		StackText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		StackText->SetVisibility(ESlateVisibility::Hidden);
	}
}
