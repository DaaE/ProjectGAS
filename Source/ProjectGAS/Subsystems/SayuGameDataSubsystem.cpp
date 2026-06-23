// Fill out your copyright notice in the Description page of Project Settings.


#include "SayuGameDataSubsystem.h"
#include "SayuGameDataSettings.h"

void USayuGameDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const USayuGameDataSettings* Settings = GetDefault<USayuGameDataSettings>())
	{
		CombatStatsTable = Settings->CombatStatsTable.LoadSynchronous();
	}

	if (CombatStatsTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SayuGameDataSubsystem] CombatStatsTable 로드 성공: %s"), *CombatStatsTable->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[SayuGameDataSubsystem] CombatStatsTable 로드 실패 - Project Settings에서 할당됐는지 확인 필요"));
	}
}

void USayuGameDataSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("[SayuGameDataSubsystem] Deinitialize 호출됨 - Subsystem 파괴됨"));

	Super::Deinitialize();
}

bool USayuGameDataSubsystem::GetCombatStats(FName RowID, FSayuCombatStatsRow& OutStats) const
{
	if (!CombatStatsTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SayuGameDataSubsystem] GetCombatStats 실패 - CombatStatsTable이 비어있음"));
		return false;
	}

	if (const FSayuCombatStatsRow* FoundRow = CombatStatsTable->FindRow<FSayuCombatStatsRow>(RowID, TEXT("GetCombatStats")))
	{
		OutStats = *FoundRow;
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("[SayuGameDataSubsystem] Row '%s'를 CombatStatsTable에서 찾지 못함"), *RowID.ToString());
	return false;
}
