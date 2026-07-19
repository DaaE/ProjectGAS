// Copyright (c) 2026 Sungmin JI. All Rights Reserved.


#include "SaytCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "SaytLogChannels.h"
#include "AbilitySystem/Abilities/SaytGameplayAbility.h"
#include "AbilitySystem/Attributes/SaytAttributeSet_Combat.h"
#include "Components/CapsuleComponent.h"
#include "Subsystems/SaytGameDataSubsystem.h"


// Sets default values
ASaytCharacterBase::ASaytCharacterBase()
{
	// === GAS 컴포넌트 생성 ===
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	// AttributeSet은 여기서 안 만든다 — 자식마다 구체 타입이
	// 다르므로(플레이어용/적용) 각 자식 생성자에서 만든다.
	
	// 모든 캐릭터(플레이어/NPC)가 공통으로 필요하니 여기서 한 번에 생성
	CombatAttributeSet = CreateDefaultSubobject<USaytAttributeSet_Combat>(TEXT("CombatAttributeSet"));
	
	// DataTable에서 가져올 ID
	CombatStatsRowID = TEXT("Player_Default");
	
	// 프레임 단위 로직이 없음 — 필요해지는 자식이 생기면 그쪽에서 켤 것.
	// 캡슐 콜리전 확인은 엔진 내장 `show Collision`으로 대체됨.
	PrimaryActorTick.bCanEverTick = false;
}

// GAS 생태계 전체 (어빌리티, GameplayCue, UI 등)가
// 이 함수를 통해 ASC에 접근해요. 단순하지만 핵심적인 함수예요.
UAbilitySystemComponent* ASaytCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ASaytCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 컨트롤러가 없는 액터(지금의 더미 Enemy처럼 AI도 안 붙은 경우)는
	// PossessedBy가 호출될 일이 없어서, 여기서도 한 번 시도한다.
	InitializeAbilitySystem();
}

void ASaytCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 플레이어 Pawn은 보통 이 경로로 초기화된다.
	InitializeAbilitySystem();
}

void ASaytCharacterBase::InitializeAbilitySystem()
{
	if (bAbilitySystemInitialized || !AbilitySystemComponent)
	{
		return;
	}

	// ASC에게 "이 액터가 주인(Owner)이자 본체(Avatar)다" 라고 초기화
	// Owner와 Avatar가 분리된 이유: PlayerState 방식에서는
	// Owner = PlayerState(영속적), Avatar = Character(리스폰마다 교체)
	// 가 다르기 때문이에요. 우리는 둘 다 Character로 동일하게 설정.
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// ASC 초기화가 끝난 직후에 어빌리티 부여
	// 순서가 중요해요 - InitAbilityActorInfo 전에 부여하면 동작 안 함
	
	// === DataTable 기반 기본 스탯 초기화 (Phase 4) ===
	if (CombatAttributeSet && !CombatStatsRowID.IsNone())
	{
		if (const USaytGameDataSubsystem* GameData = GetGameInstance()->GetSubsystem<USaytGameDataSubsystem>())
		{
			FSaytCombatStatsRow Stats;
			if (GameData->GetCombatStats(CombatStatsRowID, Stats))
			{
				CombatAttributeSet->InitMaxHealth(Stats.MaxHealth);
				CombatAttributeSet->InitHealth(Stats.MaxHealth);
				CombatAttributeSet->InitMaxMana(Stats.MaxMana);
				CombatAttributeSet->InitMana(Stats.MaxMana);
				CombatAttributeSet->InitAttackPower(Stats.AttackPower);
				CombatAttributeSet->InitDefense(Stats.Defense);

				UE_LOG(LogSaytCombat, Verbose, TEXT("%s: CombatStats 적용됨 (Row: %s, MaxHealth: %.0f, AttackPower: %.0f)"),
					*GetName(), *CombatStatsRowID.ToString(), Stats.MaxHealth, Stats.AttackPower);
			}
		}
	}

	
	GiveDefaultAbilities();
	bAbilitySystemInitialized = true;
}

void ASaytCharacterBase::GiveDefaultAbilities()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	for (TSubclassOf<USaytGameplayAbility> AbilityClass : DefaultAbilities)
	{
		if (AbilityClass)
		{
			// FGameplayAbilitySpec : "이 어빌리티를 레벨 몇으로, 어떤 InputID로
			// 캐릭터에게 부여할지"를 담는 명세 객체
			// GameplayEffectSpec과 비슷한 패턴이에요 - GAS는 뭔가를
			// "적용"하기 전에 항상 Spec(명세)을 먼저 만드는 일관된 패턴을 씁니다
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
			// 1 = 어빌리티 레벨 (스킬 레벨업 시스템 만들 때 쓰일 값)
			
			AbilitySystemComponent->GiveAbility(AbilitySpec);
			// GiveAbility : 실제로 ASC의 "보유 어빌리티 목록"에 등록
			// 이 시점부터 TryActivateAbility로 발동 가능해짐
		}
	}
}
