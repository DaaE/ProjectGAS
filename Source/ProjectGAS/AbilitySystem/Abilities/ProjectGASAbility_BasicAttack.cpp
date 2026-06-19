// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGASAbility_BasicAttack.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayTagsManager.h"

UProjectGASAbility_BasicAttack::UProjectGASAbility_BasicAttack()
{
	// InstancingPolicy는 부모 생성자에서 이미 설정됨

	// 이 어빌리티가 활성화되는 동안 부여할 태그
	// "지금 공격 중이다" 라는 상태를 GameplayTag로 표현
	// Unity에서 bool isAttacking 변수로 했을 일을
	// GAS는 태그 시스템으로 표준화함
	// (Phase 0에서 만든 DefaultGameplayTags.ini의 Ability.Attack.Light 사용)
	
	// AbilityTags : 이 어빌리티 자신을 식별하는 태그
	// TryActivateAbilitiesByTag가 바로 이 태그를 보고 어빌리티를 찾아냄
	FGameplayTagContainer NewTags;
	NewTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Light")));

	SetAssetTags(NewTags);
	// SetAssetTags() : UE5.3+ 에서 권장하는 방식
	// (예전 버전은 AbilityTags 멤버에 직접 접근했는데, 최신 API로 변경됨)
}

void UProjectGASAbility_BasicAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// 아직 Super를 호출하지 않음 - 먼저 조건 재확인

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		// CommitAbility : 코스트(마나 소모)와 쿨타임을 "실제로 소비"하는 함수
		// CanActivateAbility는 "가능한지 확인"만 하고, CommitAbility가
		// 실제 차감을 실행해요. 이 둘이 분리된 이유는 멀티플레이어에서
		// "확인"과 "실행" 사이에 다른 어빌리티가 끼어들 수 있어서예요.
	{
		UE_LOG(LogTemp, Warning, TEXT("BasicAttack Commit Failed - On Cooldown"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("BasicAttack Activated"));

	// 실제 데미지 Effect 적용
	if (DamageEffectClass && GetAbilitySystemComponentFromActorInfo())
	{
		FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(DamageEffectClass);
		// MakeOutgoingGameplayEffectSpec : GameplayAbility 안에서 쓰는 헬퍼 함수
		// Character에서 직접 했던 MakeEffectContext + MakeOutgoingSpec을
		// 한 줄로 줄여줌 (어빌리티 컨텍스트 안에서는 더 간결하게 쓸 수 있음)

		if (EffectSpec.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, EffectSpec);
			// ApplyGameplayEffectSpecToOwner : 자기 자신에게 적용
			// (테스트 단계라 Self 적용. 나중에 적 타겟팅으로 바꿀 예정)
		}
	}

	// 정상 종료
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UProjectGASAbility_BasicAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	// Super 호출이 중요해요. 부모(UGameplayAbility)의 EndAbility가
	// 내부적으로 정리 작업(태그 제거, 인스턴스 정리 등)을 하기 때문이에요.
}

UGameplayEffect* UProjectGASAbility_BasicAttack::GetCooldownGameplayEffect() const
{
	if (CooldownEffectClass)
	{
		// GetDefaultObject : 이 클래스의 CDO를 가져옴
		// GameplayEffect는 인스턴스화 없이 CDO 자체를 "명세"로 사용해요
		return CooldownEffectClass->GetDefaultObject<UGameplayEffect>();
	}
	return nullptr;
}
