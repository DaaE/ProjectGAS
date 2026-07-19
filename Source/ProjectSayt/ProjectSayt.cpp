// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectSayt.h"
#include "Modules/ModuleManager.h"
#include "UI/SaytStyle.h"

void FProjectSaytModule::StartupModule()
{
	FSaytStyle::Initialize();
}

void FProjectSaytModule::ShutdownModule()
{
	FSaytStyle::Shutdown();
}

IMPLEMENT_PRIMARY_GAME_MODULE( FProjectSaytModule, ProjectSayt, "ProjectSayt" );
