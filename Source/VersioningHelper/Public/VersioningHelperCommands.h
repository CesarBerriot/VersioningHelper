#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "VersioningHelperStyle.h"

class FVersioningHelperCommands : public TCommands<FVersioningHelperCommands>
{
public:
	FVersioningHelperCommands()
	: TCommands<FVersioningHelperCommands>(TEXT("VersioningHelper"), NSLOCTEXT("Contexts", "VersioningHelper", "VersioningHelper Plugin"), NAME_None, FVersioningHelperStyle::GetStyleSetName()) { }
	// TCommands<> interface
	virtual void RegisterCommands() override;
public:
	TSharedPtr<FUICommandInfo> OpenPluginWindow;
};
