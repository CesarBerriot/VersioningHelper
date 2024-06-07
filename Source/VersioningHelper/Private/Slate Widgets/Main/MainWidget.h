#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class VERSIONINGHELPER_API SMainWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainWidget) {}
	SLATE_END_ARGS()

	void Construct(FArguments const & InArgs);
private:
	TSharedRef<SWidget> ConstructErrorInterface();
	TSharedRef<SWidget> ConstructNormalInterface();
	TSharedRef<SWindow> ConstructManualResetPopupWindow();
	FReply IncreaseMinorVersion();
	FReply IncreasePatchVersion();
	FString MakeVersionString();
	FReply SaveVersionAsset();
	FReply CreateManualResetPopup();
	class UVersionAsset * VersionAsset = nullptr;
	TSharedPtr<SWindow> ManualResetPopupWindow;
};
