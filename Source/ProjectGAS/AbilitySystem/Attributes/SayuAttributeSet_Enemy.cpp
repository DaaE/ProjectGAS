#include "SayuAttributeSet_Enemy.h"
#include "Net/UnrealNetwork.h"

USayuAttributeSet_Enemy::USayuAttributeSet_Enemy()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitDefense(0.f);
}

void USayuAttributeSet_Enemy::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USayuAttributeSet_Enemy, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USayuAttributeSet_Enemy, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USayuAttributeSet_Enemy, Defense, COND_None, REPNOTIFY_Always);
}

void USayuAttributeSet_Enemy::OnRep_Health(
	const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(
		USayuAttributeSet_Enemy, Health, OldHealth);
}

void USayuAttributeSet_Enemy::OnRep_MaxHealth(
	const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(
		USayuAttributeSet_Enemy, MaxHealth, OldMaxHealth);
}

void USayuAttributeSet_Enemy::OnRep_Defense(
	const FGameplayAttributeData& OldDefense)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(
		USayuAttributeSet_Enemy, Defense, OldDefense);
}

void USayuAttributeSet_Enemy::PreAttributeChange(
	const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}