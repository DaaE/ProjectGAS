// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectGASGameplayAbility.h"
#include "ProjectGASAbility_BasicAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAS_API UProjectGASAbility_BasicAttack : public UProjectGASGameplayAbility
{
	GENERATED_BODY()
	
public:
	UProjectGASAbility_BasicAttack();

protected:
	// ActivateAbility : 어빌리티가 실제로 "발동"될 때 호출되는 함수
	// CanActivateAbility 체크를 통과한 후에만 호출됨 (자동)
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	// 매개변수 4개가 다 낯설 수 있는데, 지금은 패턴으로 외우셔도 돼요.
	// Handle      : 이 어빌리티 인스턴스를 가리키는 식별자
	// ActorInfo   : 누가 이 어빌리티를 쓰는지 (Avatar, Owner 등 정보)
	// ActivationInfo : 예측 관련 정보 (Phase 2-3에서 얘기했던 PredictionKey 포함)
	// TriggerEventData : GameplayEvent로 트리거된 경우의 데이터 (지금은 안 씀)

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;
	
	// 쿨타임으로 쓸 GameplayEffect 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
	TSubclassOf<UGameplayEffect> CooldownEffectClass;

	// UGameplayAbility가 CanActivateAbility 체크 시 자동으로 호출하는 함수
	virtual UGameplayEffect* GetCooldownGameplayEffect() const override;
};
