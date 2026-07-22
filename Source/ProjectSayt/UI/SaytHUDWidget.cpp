// Copyright (c) 2026 Sungmin JI. All Rights Reserved.


#include "SaytHUDWidget.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/NativeWidgetHost.h"
#include "UI/SaytHealthDisplayTypes.h"
#include "UI/Slate/SSaytHealthDisplay.h"

void USaytHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (HealthBarHost)
	{
		// enum → 프리셋 조회 → 개별 값 전달. 위젯은 타입을 모른다
		const FSaytHealthDisplayPreset Preset = GetSaytHealthDisplayPreset(ESaytHealthDisplayType::Player);
		HealthBar = SNew(SSaytHealthDisplay)
			.BarStyle(Preset.BarStyle)
			.DesiredBarSize(Preset.BarSize);
		
		HealthBarHost->SetContent(HealthBar.ToSharedRef());
		
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwningPlayerPawn()))
		{
			HealthBar->BindToASC(ASC);
		}
	}
}

void USaytHUDWidget::NativeDestruct()
{
	// 대칭 해제 — Slate 바 소멸자도 스스로 풀지만, 소유자가 명시적으로 정리하는 규율 유지
	if (HealthBar.IsValid())
	{
		HealthBar->UnbindFromASC();
	}
	HealthBar.Reset();
	
	Super::NativeDestruct();
}