// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGASGameplayAbility.h"

UProjectGASGameplayAbility::UProjectGASGameplayAbility()
{
	// InstancingPolicy : 이 어빌리티가 발동될 때마다 새 인스턴스를 만들지,
	// 캐릭터당 하나만 만들지를 결정해요.
	// Unity에는 없는 개념인데, GAS는 어빌리티 자체가 "객체"라서
	// 메모리/상태 관리 방식을 선택해야 해요.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	// InstancedPerActor : 캐릭터 한 명당 어빌리티 인스턴스 하나
	// (다른 옵션: InstancedPerExecution - 발동마다 새로 생성, NonInstanced - 공유)
}
