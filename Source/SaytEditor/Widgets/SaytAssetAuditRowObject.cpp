// Copyright (c) 2026 Sungmin JI. All Rights Reserved.


#include "SaytAssetAuditRowObject.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "SaytEditor/SaytEditor.h"

void USaytAssetAuditRowObject::LoadAllBundles()
{
	if (!UAssetManager::IsInitialized() || !AssetId.IsValid())
	{
		return;
	}

	const FPrimaryAssetId LoadingId = AssetId;
	// Phase 5에서 쓴 FStreamableManager 직접 호출과 다른 점:
	// 여기선 AssetManager가 "이 PrimaryAsset이 가진 Bundle들"을 알아서 풀어서
	// 그 안의 모든 소프트 레퍼런스를 한 번에 로드해준다 (내부적으로는 같은 StreamableManager를 씀).
	LoadHandle = UAssetManager::Get().LoadPrimaryAsset(AssetId, Bundles, 
		FStreamableDelegate::CreateLambda([LoadingId]()
		{
			UE_LOG(LogSaytEditor, Log, TEXT("[Sayt] Bundle 로드 완료: %s"), *LoadingId.ToString());
		}));
}

void USaytAssetAuditRowObject::UnloadAsset()
{
	if (!UAssetManager::IsInitialized() || !AssetId.IsValid())
	{
		return;
	}

	UAssetManager::Get().UnloadPrimaryAsset(AssetId);
	LoadHandle.Reset();
}

bool USaytAssetAuditRowObject::IsLoaded() const
{
	return LoadHandle.IsValid() && LoadHandle->HasLoadCompleted();
}
