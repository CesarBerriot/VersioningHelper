#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class VERSIONINGHELPER_API SManualResetWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SManualResetWidget) {}
	SLATE_EVENT(FSimpleDelegate, OnConfirmed)
	SLATE_END_ARGS()

	void Construct(FArguments const & InArgs, class UVersionAsset * InVersionAsset);
private:
	FReply Confirm();
	void UpdateRequestedMinorVersion(int NewValue, ETextCommit::Type CommitType) { RequestedMinorVersion = NewValue; }
	void UpdateRequestedPatchVersion(int NewValue, ETextCommit::Type CommitType) { RequestedPatchVersion = NewValue; }
	UVersionAsset * VersionAsset = nullptr;
	FSimpleDelegate OnConfirmed;
	int RequestedMinorVersion;
	int RequestedPatchVersion;
};
