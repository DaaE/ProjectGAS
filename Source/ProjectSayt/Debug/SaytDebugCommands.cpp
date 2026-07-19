// Copyright (c) 2026 Sungmin JI. All Rights Reserved.

// ─────────────────────────────────────────────────────────────
// 프로젝트 디버그 콘솔 커맨드 집합소
//
// 이 파일은 헤더가 없습니다 — 아무도 이 파일을 include할 수 없으므로
// 의존 방향이 항상 [디버그 → 도메인] 단방향으로 강제됩니다.
// FAutoConsoleCommand 계열은 파일 스코프 정적 객체의 생성자에서
// 스스로 콘솔에 등록되고 소멸자에서 해제되는 RAII 객체라, 별도의
// 초기화 호출 없이 이 파일이 링크되는 것만으로 커맨드가 살아납니다.
//
// [Phase 12 정리 대상] 아래 커맨드들은 개발 검증용입니다.
// ─────────────────────────────────────────────────────────────

#if !UE_BUILD_SHIPPING

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "HAL/IConsoleManager.h"
#include "SaytLogChannels.h"
#include "UI/Slate/SSaytHealthBar.h"
#include "UI/Slate/SSaytTuningPanel.h"
#include "Widgets/Layout/SBox.h"

// ═════════════════════════════════════════════════════════════
// Phase 8 Stage 0 — Slate 라이브 튜닝 데모 패널
// ═════════════════════════════════════════════════════════════
namespace SaytTuningDemo
{
	static TSharedPtr<SSaytTuningPanel> ActivePanel;
	static FDelegateHandle WorldCleanupHandle;

	static void OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
	{
		// PIE가 끝나면(뷰포트가 사라지면) 우리 포인터도 같이 비워서,
		// 다음 PIE 세션이 '이전 세션의 유령'을 진짜로 살아있다고 착각하지 않게 합니다.
		ActivePanel.Reset();
	}

	static void ToggleTuningDemo()
	{
		if (!GEngine || !GEngine->GameViewport)
		{
			return;
		}

		if (!WorldCleanupHandle.IsValid())
		{
			WorldCleanupHandle = FWorldDelegates::OnWorldCleanup.AddStatic(&OnWorldCleanup);
		}

		if (ActivePanel.IsValid())
		{
			GEngine->GameViewport->RemoveViewportWidgetContent(ActivePanel.ToSharedRef());
			ActivePanel.Reset();
		}
		else
		{
			GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(ActivePanel, SSaytTuningPanel));
		}
	}

	static FAutoConsoleCommand ToggleTuningDemoCommand(
		TEXT("Sayt.ToggleTuningDemo"),
		TEXT("Phase 8 Stage 0 Slate 라이브 튜닝 데모 패널을 켜고 끕니다."),
		FConsoleCommandDelegate::CreateStatic(&ToggleTuningDemo)
	);
}

// ═════════════════════════════════════════════════════════════
// Phase 8 Stage 1 — 체력바 검증 (Stage 2~5 검증에도 계속 사용)
// ═════════════════════════════════════════════════════════════
namespace SaytHealthBarDebug
{
	static TSharedPtr<SWidget> BarRoot;
	static TWeakPtr<SSaytHealthBar> BarWidget;

	static FAutoConsoleCommandWithWorld ShowCmd(
		TEXT("Sayt.HealthBar.Show"),
		TEXT("플레이어 ASC에 바인딩된 SSaytHealthBar를 좌하단에 토글"),
		FConsoleCommandWithWorldDelegate::CreateLambda([](UWorld* World)
		{
			UGameViewportClient* Viewport = World ? World->GetGameViewport() : nullptr;
			if (!Viewport) { return; }

			if (BarRoot.IsValid())
			{
				Viewport->RemoveViewportWidgetContent(BarRoot.ToSharedRef());
				BarRoot.Reset();
				return;
			}

			APlayerController* PC = World->GetFirstPlayerController();
			APawn* Pawn = PC ? PC->GetPawn() : nullptr;
			// 캐릭터 클래스명에 의존하지 않는 표준 ASC 조회 경로 (IAbilitySystemInterface 경유)
			UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);
			if (!ASC) { UE_LOG(LogSaytUI, Warning, TEXT("[HealthBar.Show] 플레이어 ASC를 찾지 못함")); return; }

			TSharedPtr<SSaytHealthBar> Bar;
			BarRoot =
				SNew(SBox).HAlign(HAlign_Left).VAlign(VAlign_Bottom).Padding(60.f, 0.f, 0.f, 60.f)
				[
					SAssignNew(Bar, SSaytHealthBar)
				];
			Viewport->AddViewportWidgetContent(BarRoot.ToSharedRef());
			Bar->BindToASC(ASC);
			BarWidget = Bar;
		})
	);

	// 줄 색 스왑 시뮬레이션: Sayt.HealthBar.Tint <R> <G> <B> (0~1)
	static FAutoConsoleCommandWithWorldAndArgs TintCmd(
		TEXT("Sayt.HealthBar.Tint"),
		TEXT("표시 중인 체력바의 채움 틴트를 변경 (TSlateAttribute 검증용)"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld*)
		{
			TSharedPtr<SSaytHealthBar> Bar = BarWidget.Pin();
			if (!Bar.IsValid() || Args.Num() < 3) { return; }
			Bar->SetFillTint(FLinearColor(
				FCString::Atof(*Args[0]), FCString::Atof(*Args[1]), FCString::Atof(*Args[2])));
		})
	);

	static FDelegateHandle CleanupHandle = FWorldDelegates::OnWorldCleanup.AddLambda(
		[](UWorld*, bool, bool) { BarRoot.Reset(); });
}

#endif // !UE_BUILD_SHIPPING