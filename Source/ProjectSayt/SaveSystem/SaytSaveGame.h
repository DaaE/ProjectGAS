// Copyright (c) 2026 Sungmin JI. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaytSaveGame.generated.h"

// 저장하고 싶은 값만 직접 고른 "데이터 가방". 월드 스냅샷이 아님.
UCLASS()
class PROJECTSAYT_API USaytSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	float SavedHealth = 0.f;

	UPROPERTY()
	float SavedMana = 0.f;
};
