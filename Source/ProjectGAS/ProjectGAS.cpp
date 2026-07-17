// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectGAS.h"
#include "Modules/ModuleManager.h"
#include "UI/SayuStyle.h"

void FProjectGASModule::StartupModule()
{
	FSayuStyle::Initialize();
}

void FProjectGASModule::ShutdownModule()
{
	FSayuStyle::Shutdown();
}

IMPLEMENT_PRIMARY_GAME_MODULE( FProjectGASModule, ProjectGAS, "ProjectGAS" );
