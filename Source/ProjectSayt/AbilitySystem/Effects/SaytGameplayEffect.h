// Copyright (c) 2026 Sungmin JI. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "SaytGameplayEffect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSAYT_API USaytGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	USaytGameplayEffect();
	// 지금은 비어있어도 됨. 나중에 공통 초기화 로직 추가할 확장 포인트
};
