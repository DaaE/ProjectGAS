// Copyright (c) 2026 Sungmin JI. All Rights Reserved.

#include "SaytHealthDisplayTypes.h"
#include "SaytStyle.h"
#include "Slate/SaytHealthBarWidgetStyle.h"

FSaytHealthDisplayPreset GetSaytHealthDisplayPreset(ESaytHealthDisplayType InType)
{
	FSaytHealthDisplayPreset Preset;
	switch (InType)
	{
	case ESaytHealthDisplayType::Boss:
		Preset.BarStyle = &FSaytStyle::Get().GetWidgetStyle<FSaytHealthBarStyle>("Sayt.HealthBar.Boss");
		Preset.BarSize = FVector2D(700.f, 30.f);
		break;

	case ESaytHealthDisplayType::Mob:
		Preset.BarStyle = &FSaytStyle::Get().GetWidgetStyle<FSaytHealthBarStyle>("Sayt.HealthBar.Mob");
		Preset.BarSize = FVector2D(90.f, 10.f);
		break;

	case ESaytHealthDisplayType::Player:
	default:
		// 기존 키를 Player 룩으로 재사용 — 기존 폴백 경로와의 호환 유지 (개명은 추후 잡무)
		Preset.BarStyle = &FSaytStyle::Get().GetWidgetStyle<FSaytHealthBarStyle>("Sayt.HealthBar");
		Preset.BarSize = FVector2D(400.f, 32.f);
		break;
	}
	return Preset;
}