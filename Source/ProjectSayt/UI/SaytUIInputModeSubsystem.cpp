// Copyright (c) 2026 Sungmin JI. All Rights Reserved.


#include "SaytUIInputModeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void USaytUIInputModeSubsystem::PushUIRequest(FName RequesterId)
{
	ActiveRequesters.Add(RequesterId);
	ApplyCurrentMode();
}

void USaytUIInputModeSubsystem::PopUIRequest(FName RequesterId)
{
	ActiveRequesters.Remove(RequesterId);
	ApplyCurrentMode();
}

void USaytUIInputModeSubsystem::ApplyCurrentMode()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		return;
	}

	if (ActiveRequesters.Num() > 0)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
}
