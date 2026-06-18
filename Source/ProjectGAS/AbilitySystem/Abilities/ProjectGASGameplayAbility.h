// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ProjectGASGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAS_API UProjectGASGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UProjectGASGameplayAbility();
	
	// 모든 스킬이 공통으로 가질 데미지 Effect 클래스 참조
	// 자식 클래스(블루프린트)에서 다른 데미지 값으로 오버라이드 가능
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};

