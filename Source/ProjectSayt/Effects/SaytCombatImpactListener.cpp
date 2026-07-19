// Copyright (c) 2026 Sungmin JI. All Rights Reserved.


#include "SaytCombatImpactListener.h"

#include "SaytEffectPoolManager.h"
#include "Messages/SaytCombatMessages.h"


// Sets default values
ASaytCombatImpactListener::ASaytCombatImpactListener()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASaytCombatImpactListener::BeginPlay()
{
	Super::BeginPlay();
	
	DamageListenerHandle = UGameplayMessageSubsystem::Get(this).RegisterListener(
		TAG_Message_Combat_Damage_Dealt, this, &ASaytCombatImpactListener::OnDamageMessage);
}

void ASaytCombatImpactListener::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DamageListenerHandle.Unregister();
	Super::EndPlay(EndPlayReason);
}

void ASaytCombatImpactListener::OnDamageMessage(FGameplayTag Channel, const FSaytDamageMessage& Message)
{
	if (!HitEffectPool || !Message.Target)
	{
		return;
	}

	const FVector Location = Message.HitLocation;
	const FRotator Rotation = Message.Target->GetActorRotation();
	const float Pitch = Message.bIsCritical ? CriticalPitchMultiplier : 1.f;

	HitEffectPool->PlayEffectAtLocation(Location, Rotation, Pitch);
}