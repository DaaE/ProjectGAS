// Copyright (c) 2026 Sungmin JI. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/SaytGameplayAbility.h"
#include "SaytAbility_BuffSelf.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSAYT_API USaytAbility_BuffSelf : public USaytGameplayAbility
{
	GENERATED_BODY()
	
public:
	USaytAbility_BuffSelf();

protected:
	// 버프로 사용할 Effect (에디터에서 GE_Buff_AttackUp 연결할 거예요)
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> BuffEffectClass;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;
};
