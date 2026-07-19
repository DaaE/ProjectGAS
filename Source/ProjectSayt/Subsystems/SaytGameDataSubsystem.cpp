// Copyright (c) 2026 Sungmin JI. All Rights Reserved.


#include "SaytGameDataSubsystem.h"
#include "SaytGameDataSettings.h"
#include "SaytLogChannels.h"
#include "AbilitySystem/Attributes/SaytAttributeSet_Combat.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/SaytSaveGame.h"

void USaytGameDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const USaytGameDataSettings* Settings = GetDefault<USaytGameDataSettings>())
	{
		CombatStatsTable = Settings->CombatStatsTable.LoadSynchronous();
	}

	if (CombatStatsTable)
	{
		UE_LOG(LogSaytData, Log, TEXT("[SaytGameDataSubsystem] CombatStatsTable 로드 성공: %s"), *CombatStatsTable->GetName());
	}
	else
	{
		UE_LOG(LogSaytData, Error, TEXT("[SaytGameDataSubsystem] CombatStatsTable 로드 실패 - Project Settings에서 할당됐는지 확인 필요"));
	}
}

void USaytGameDataSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool USaytGameDataSubsystem::GetCombatStats(FName RowID, FSaytCombatStatsRow& OutStats) const
{
	if (!CombatStatsTable)
	{
		UE_LOG(LogSaytData, Warning, TEXT("[SaytGameDataSubsystem] GetCombatStats 실패 - CombatStatsTable이 비어있음"));
		return false;
	}

	if (const FSaytCombatStatsRow* FoundRow = CombatStatsTable->FindRow<FSaytCombatStatsRow>(RowID, TEXT("GetCombatStats")))
	{
		OutStats = *FoundRow;
		return true;
	}

	UE_LOG(LogSaytData, Warning, TEXT("[SaytGameDataSubsystem] Row '%s'를 CombatStatsTable에서 찾지 못함"), *RowID.ToString());
	return false;
}

void USaytGameDataSubsystem::SaveCombatState(USaytAttributeSet_Combat* AttributeSet, const FString& SlotName)
{
	if (!AttributeSet)
	{
		return;
	}

	USaytSaveGame* SaveGameObject = Cast<USaytSaveGame>(UGameplayStatics::CreateSaveGameObject(USaytSaveGame::StaticClass()));
	if (!SaveGameObject)
	{
		return;
	}

	SaveGameObject->SavedHealth = AttributeSet->GetHealth();
	SaveGameObject->SavedMana = AttributeSet->GetMana();

	UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, 0);

	UE_LOG(LogSaytData, Log, TEXT("[SaytGameDataSubsystem] 저장됨 (Slot: %s, Health: %.1f, Mana: %.1f)"),
		*SlotName, SaveGameObject->SavedHealth, SaveGameObject->SavedMana);
}

bool USaytGameDataSubsystem::LoadCombatState(USaytAttributeSet_Combat* AttributeSet, const FString& SlotName)
{
	if (!AttributeSet || !UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		UE_LOG(LogSaytData, Warning, TEXT("[SaytGameDataSubsystem] 로드 실패 - 슬롯 '%s' 없음"), *SlotName);
		return false;
	}

	const USaytSaveGame* SaveGameObject = Cast<USaytSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (!SaveGameObject)
	{
		return false;
	}

	AttributeSet->InitHealth(SaveGameObject->SavedHealth);
	AttributeSet->InitMana(SaveGameObject->SavedMana);

	UE_LOG(LogSaytData, Log, TEXT("[SaytGameDataSubsystem] 로드됨 (Slot: %s, Health: %.1f, Mana: %.1f)"),
		*SlotName, SaveGameObject->SavedHealth, SaveGameObject->SavedMana);

	return true;
}
