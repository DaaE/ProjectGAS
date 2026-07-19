// Copyright (c) 2026 Sungmin JI. All Rights Reserved.


#include "SaytItemIdSubsystem.h"

int32 USaytItemIdSubsystem::IssueNextItemInstanceId()
{
	return NextItemInstanceId++;
}
