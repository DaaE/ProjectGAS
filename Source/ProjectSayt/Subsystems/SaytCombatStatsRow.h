#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SaytCombatStatsRow.generated.h"

USTRUCT(BlueprintType)
struct FSaytCombatStatsRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMana = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackPower = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Defense = 0.f;
};