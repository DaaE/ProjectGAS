// Copyright (c) 2026 Sungmin JI. All Rights Reserved.


#include "Effects/SaytEffectPoolManager.h"

#include "SaytLogChannels.h"
#include "SaytPooledEffect.h"

// Sets default values
ASaytEffectPoolManager::ASaytEffectPoolManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASaytEffectPoolManager::BeginPlay()
{
	Super::BeginPlay();
	
	for (int32 i = 0; i < InitialPoolSize; ++i)
	{
		SpawnPooledEffect();
	}
}

ASaytPooledEffect* ASaytEffectPoolManager::SpawnPooledEffect()
{
	if (!EffectClass)
	{
		return nullptr;
	}

	ASaytPooledEffect* NewEffect = GetWorld()->SpawnActor<ASaytPooledEffect>(EffectClass);
	if (NewEffect)
	{
		Pool.Add(NewEffect);
	}
	return NewEffect;
}

void ASaytEffectPoolManager::PlayEffectAtLocation(const FVector& Location, const FRotator& Rotation, float PitchMultiplier)
{
	// 풀에서 지금 안 쓰이고 있는(숨겨진) 인스턴스를 찾음
	// - ASaytHitImpactEffect가 평소엔 SetActorHiddenInGame(true) 상태라
	//   IsHidden()이 곧 "지금 비어있나"의 신호가 됨
	for (ASaytPooledEffect* Effect : Pool)
	{
		if (Effect && Effect->IsHidden())
		{
			Effect->PlayEffect(Location, Rotation, PitchMultiplier);
			return;
		}
	}

	// 다 빌려나가서 빈 게 없으면 - 새로 하나 더 만들어서 씀
	// (이 로그가 자주 뜨면 InitialPoolSize를 늘리라는 신호)
	UE_LOG(LogSaytCombat, Warning, TEXT("EffectPool 부족 - 풀 크기를 늘리세요. 현재: %d"), Pool.Num());

	if (ASaytPooledEffect* NewEffect = SpawnPooledEffect())
	{
		NewEffect->PlayEffect(Location, Rotation, PitchMultiplier);
	}
}