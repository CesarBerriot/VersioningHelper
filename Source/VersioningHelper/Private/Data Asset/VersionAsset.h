#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VersionAsset.generated.h"

UCLASS()
class VERSIONINGHELPER_API UVersionAsset : public UDataAsset
{
	GENERATED_BODY()
private:
	UPROPERTY() int MinorVersion;
	UPROPERTY() int PatchVersion;
};
