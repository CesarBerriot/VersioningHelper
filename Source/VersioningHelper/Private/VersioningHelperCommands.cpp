#include "VersioningHelperCommands.h"

#define LOCTEXT_NAMESPACE "FVersioningHelperModule"

void FVersioningHelperCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "VersioningHelper", "Bring up VersioningHelper window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
