// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "SayuInventoryItemWidget.generated.h"

class UTextBlock;
class UImage;
class USayuItemInstance;

/**
 * 아이템 하나(=Entry 하나)를 표현하는 위젯. 그리드 배경(SlotWidget)과 별도 레이어에
 * 절대 좌표로 배치됨 — 여러 칸을 차지하는 아이템도 큰 아이콘 하나로 자연스럽게 표현.
 */
UCLASS()
class PROJECTGAS_API USayuInventoryItemWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItemInstance(USayuItemInstance* Instance);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IconImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StackText;
};
