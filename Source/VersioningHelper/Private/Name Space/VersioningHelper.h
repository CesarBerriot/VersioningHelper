#pragma once
#include "Data Asset/VersionAsset.h"

namespace VersioningHelper
{
	FString const ContentPath = "/VersioningHelper/";
	FString const VersionAssetName = "VersionAsset";
	void AddNotification(FString Text, FString SubText);
	UVersionAsset * GetOrCreateVersionAsset();
	/// This function assumes the version asset doesn't exist. Do not use directly.
	UVersionAsset * CreateVersionAsset();
	int & GetVersionRef(UVersionAsset * VersionAsset, FString Name);
	inline int & GetMinorVersionRef(UVersionAsset * VersionAsset) { return GetVersionRef(VersionAsset, "MinorVersion"); }
	inline int & GetPatchVersionRef(UVersionAsset * VersionAsset) { return GetVersionRef(VersionAsset, "PatchVersion"); }
	void IncreaseMinorVersion(UVersionAsset * VersionAsset);
	void IncreasePatchVersion(UVersionAsset * VersionAsset);
	void SaveVersionAsset(UVersionAsset * VersionAsset);
	void SetVersions(UVersionAsset * VersionAsset, int MinorVersion, int PatchVersion);
}
