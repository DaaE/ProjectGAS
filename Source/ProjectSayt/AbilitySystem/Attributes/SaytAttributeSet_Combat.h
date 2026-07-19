// Copyright (c) 2026 Sungmin JI. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SaytAttributeSetMacros.h"
#include "SaytAttributeSet_Combat.generated.h"

// 전투에 관여하는 모든 액터(플레이어/NPC 공통)가 붙이는 독립 AttributeSet.
// 상속이 아니라 ASC에 직접 붙는 합성(composition) 방식으로 공유됨.
UCLASS()
class PROJECTSAYT_API USaytAttributeSet_Combat : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	USaytAttributeSet_Combat();
	
	// === 체력 ===
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USaytAttributeSet_Combat, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USaytAttributeSet_Combat, MaxHealth)
	
	// === 마나 ===
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(USaytAttributeSet_Combat, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(USaytAttributeSet_Combat, MaxMana)
	
	// === 전투 스탯 ===
	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_AttackPower)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(USaytAttributeSet_Combat, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(USaytAttributeSet_Combat, Defense)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_ComboCount)
	FGameplayAttributeData ComboCount;
	ATTRIBUTE_ACCESSORS(USaytAttributeSet_Combat, ComboCount)
	
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION() virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION() virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION() virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);
	UFUNCTION() virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);
	UFUNCTION() virtual void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower);
	UFUNCTION() virtual void OnRep_Defense(const FGameplayAttributeData& OldDefense);
	UFUNCTION() virtual void OnRep_ComboCount(const FGameplayAttributeData& OldComboCount);
	

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// 여기서 PostGameplayEffectExecute까지 같이 처리 -
	// Health 변경 로그/클램프를 Enemy 클래스에만 두지 않고 베이스에서 공통 처리
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
