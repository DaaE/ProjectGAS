// Copyright (c) 2026 Sungmin JI. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

struct FSaytHealthBarStyle;

// 표시 타입 — 룩(스타일/크기)을 결정한다. 세그먼트 수는 개체 데이터 소관이라
// 여기 없음: 같은 Boss 타입에 세그먼트 1짜리와 4짜리가 공존해야 하므로.
// (Phase 11에서 DT 컬럼으로 노출할 때 UENUM 승격 검토)
enum class ESaytHealthDisplayType : uint8
{
	Player,
	Boss,
	Mob
};

// 타입 → 설정 묶음. enum은 이 조회 계층까지만 내려온다 —
// 위젯은 여기서 풀린 개별 값만 받고, 게임 어휘(보스/몹)를 모른다.
struct FSaytHealthDisplayPreset
{
	const FSaytHealthBarStyle* BarStyle = nullptr;
	FVector2D BarSize = FVector2D(400.f, 32.f);
};

FSaytHealthDisplayPreset GetSaytHealthDisplayPreset(ESaytHealthDisplayType InType);