// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectGASHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAS_API UProjectGASHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// NativeTick : 매 프레임 호출되는 함수 (UMG 전용 Tick)
	// C#의 Update()와 비슷하지만, 위젯이 화면에 보일 때만 호출됨
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// BlueprintReadOnly : 블루프린트에서 이 값을 읽어서 TextBlock에 표시할 거예요
	// C++ 에서는 값을 계산만 하고, 실제 화면 배치는 블루프린트가 담당
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float CurrentHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float MaxHealth;
};
