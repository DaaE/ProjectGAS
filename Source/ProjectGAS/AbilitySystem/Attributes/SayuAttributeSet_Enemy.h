#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ProjectGASAttributeSetMacros.h"
#include "SayuAttributeSet_Enemy.generated.h"

UCLASS()
class PROJECTGAS_API USayuAttributeSet_Enemy : public UAttributeSet
{
	GENERATED_BODY()

public:
	USayuAttributeSet_Enemy();

	UPROPERTY(BlueprintReadOnly, Category = "Health",
		ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USayuAttributeSet_Enemy, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health",
		ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USayuAttributeSet_Enemy, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Combat",
		ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(USayuAttributeSet_Enemy, Defense)

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Defense(const FGameplayAttributeData& OldDefense);

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};