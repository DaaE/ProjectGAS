// Copyright (c) 2026 Sungmin JI. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "SaytCombatImpactListener.generated.h"

class ASaytEffectPoolManager;
struct FSaytDamageMessage;

UCLASS()
class PROJECTSAYT_API ASaytCombatImpactListener : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASaytCombatImpactListener();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void OnDamageMessage(FGameplayTag Channel, const FSaytDamageMessage& Message);

	// 레벨에 배치한, 타격 SFX 전용으로 설정된 풀 매니저 (스윙 이펙트용과는 별개 인스턴스)
	UPROPERTY(EditInstanceOnly, Category = "Sayt|Combat")
	TObjectPtr<ASaytEffectPoolManager> HitEffectPool;

	// 지금은 사운드가 없어서 효과가 안 보이지만, 나중에 타격 사운드가 생기면
	// 크리티컬일 때 음높이를 살짝 올려서 구분할 자리로 남겨둡니다.
	UPROPERTY(EditDefaultsOnly, Category = "Sayt|Combat")
	float CriticalPitchMultiplier = 1.3f;

private:
	FGameplayMessageListenerHandle DamageListenerHandle;
};
