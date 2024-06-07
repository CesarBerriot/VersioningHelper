#include "VersioningHelper.h"

#include "AssetToolsModule.h"
#include "EngineUtils.h"
#include "ObjectTools.h"
#include "Factories/DataAssetFactory.h"
#include "Framework/Notifications/NotificationManager.h"
#include "UObject/SavePackage.h"
#include "Widgets/Notifications/SNotificationList.h"

void VersioningHelper::AddNotification(FString Text, FString SubText = FString())
{
	FNotificationInfo Info(FText::FromString(Text));
	Info.SubText = FText::FromString(SubText);
	FSlateNotificationManager::Get().AddNotification(Info);
}

UVersionAsset * VersioningHelper::GetOrCreateVersionAsset()
{
	TArray<UObject*> FoundAssets;
	verify(EngineUtils::FindOrLoadAssetsByPath(ContentPath, FoundAssets, EngineUtils::ATL_Regular));
	for(UObject * FoundAsset : FoundAssets)
		if(FoundAsset->GetName() == VersionAssetName)
		{
			if(Cast<UVersionAsset>(FoundAsset))
				return (UVersionAsset*)FoundAsset;
			else
			{
				bool Success = ObjectTools::DeleteSingleObject(FoundAsset, false);
				if(!Success)
				{
					AddNotification("Failed to delete an invalid asset", FString::Printf(L"Path : %ls.uasset", *(ContentPath / VersionAssetName)));
					return nullptr;
				}
				return CreateVersionAsset();
			}
		}
	AddNotification("The version asset doesn't exist");
	return CreateVersionAsset();
}

UVersionAsset * VersioningHelper::CreateVersionAsset()
{
	IAssetTools & AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	UDataAssetFactory * Factory = NewObject<UDataAssetFactory>();
	UObject * Asset = AssetTools.CreateAsset(VersionAssetName, ContentPath, UVersionAsset::StaticClass(), Factory);
	if(!Asset)
	{
		AddNotification("Failed to create the version asset", FString("Path : ") + ContentPath / VersionAssetName);
		return nullptr;
	}
	else
	{
		AddNotification("Successfully created the version asset");
		return (UVersionAsset*)Asset;
	}
}

int & VersioningHelper::GetVersionRef(UVersionAsset * VersionAsset, FString Name)
{
	FProperty * Property = UVersionAsset::StaticClass()->FindPropertyByName(FName(Name));
	check(Property)
	int * Ptr = Property->ContainerPtrToValuePtr<int>(VersionAsset);
	check(Ptr)
	return *Ptr;
}

void VersioningHelper::IncreaseMinorVersion(UVersionAsset * VersionAsset)
{
	int & MinorVersion = GetMinorVersionRef(VersionAsset);
	int & PatchVersion = GetPatchVersionRef(VersionAsset);
	++MinorVersion;
	PatchVersion = 0;
	VersionAsset->MarkPackageDirty();
}

void VersioningHelper::IncreasePatchVersion(UVersionAsset * VersionAsset)
{
	int & PatchVersion = GetPatchVersionRef(VersionAsset);
	++PatchVersion;
	VersionAsset->MarkPackageDirty();
}

void VersioningHelper::SaveVersionAsset(UVersionAsset * VersionAsset)
{
	FSavePackageArgs SavePackageArgs;
	SavePackageArgs.TopLevelFlags = RF_Public | RF_Standalone;
	bool Success = UPackage::Save(VersionAsset->GetPackage(), VersionAsset, *(ContentPath / VersionAssetName), SavePackageArgs).IsSuccessful();
	if(Success)
		AddNotification("Successfully saved the version asset");
	else
		AddNotification("Failed to save the version asset");
}

void VersioningHelper::SetVersions(UVersionAsset * VersionAsset, int MinorVersion, int PatchVersion)
{
	GetMinorVersionRef(VersionAsset) = MinorVersion;
	GetPatchVersionRef(VersionAsset) = PatchVersion;
	VersionAsset->MarkPackageDirty();
}
