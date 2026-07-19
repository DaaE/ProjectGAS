// Copyright (c) 2026 Sungmin JI. All Rights Reserved.


#include "SaytItemInstance.h"
#include "SaytItemIdSubsystem.h"

USaytItemInstance* USaytItemInstance::CreateInstance(UObject* Outer, USaytItemDefinition* Definition)
{
	if (!Outer || !Definition)
	{
		return nullptr;
	}

	USaytItemInstance* NewInstance = NewObject<USaytItemInstance>(Outer);
	NewInstance->ItemDefinition = Definition;

	if (UWorld* World = Outer->GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (USaytItemIdSubsystem* IdSubsystem = GameInstance->GetSubsystem<USaytItemIdSubsystem>())
			{
				NewInstance->ItemInstanceId = IdSubsystem->IssueNextItemInstanceId();
			}
		}
	}

	return NewInstance;
}
