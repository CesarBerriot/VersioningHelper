#include "VersioningHelper.h"
#include "VersioningHelperStyle.h"
#include "VersioningHelperCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Slate Widgets/Main/MainWidget.h"

FName const VersioningHelperTabName("VersioningHelper");

#define LOCTEXT_NAMESPACE "FVersioningHelperModule"

void FVersioningHelperModule::StartupModule()
{
	FVersioningHelperStyle::Initialize();
	FVersioningHelperStyle::ReloadTextures();

	FVersioningHelperCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FVersioningHelperCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FVersioningHelperModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FVersioningHelperModule::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(VersioningHelperTabName, FOnSpawnTab::CreateRaw(this, &FVersioningHelperModule::OnSpawnPluginTab))
	                        .SetDisplayName(LOCTEXT("FVersioningHelperTabTitle", "VersioningHelper"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FVersioningHelperModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FVersioningHelperStyle::Shutdown();

	FVersioningHelperCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(VersioningHelperTabName);
}

TSharedRef<SDockTab> FVersioningHelperModule::OnSpawnPluginTab(FSpawnTabArgs const & SpawnTabArgs)
{
	return SNew(SDockTab)
	       .TabRole(ETabRole::NomadTab)
	       [
		       SNew(SMainWidget)
	       ];
}

void FVersioningHelperModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(VersioningHelperTabName);
}

void FVersioningHelperModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu * Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection & Section = Menu->FindOrAddSection("Versioning Helper");
			Section.AddMenuEntryWithCommandList(FVersioningHelperCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu * ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection & Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry & Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FVersioningHelperCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVersioningHelperModule, VersioningHelper)
