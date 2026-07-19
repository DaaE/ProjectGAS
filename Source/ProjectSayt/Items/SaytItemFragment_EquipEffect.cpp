// Copyright (c) 2026 Sungmin JI. All Rights Reserved.


#include "SaytItemFragment_EquipEffect.h"
#include "AbilitySystemComponent.h"
#include "Character/SaytCharacterBase.h"
#include "GameplayEffect.h"

void USaytItemFragment_EquipEffect::OnEquipped(ASaytCharacterBase* OwningCharacter) const
{
	if (!OwningCharacter || !EffectToApply)
	{
		return;
	}

	UAbilitySystemComponent* ASC = OwningCharacter->GetAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	const FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle =
		ASC->MakeOutgoingSpec(EffectToApply, 1.f, ContextHandle);

	if (SpecHandle.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}
