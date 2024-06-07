#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FVersioningHelperModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	void PluginButtonClicked();
	void RegisterMenus();
	TSharedRef<class SDockTab> OnSpawnPluginTab(class FSpawnTabArgs const & SpawnTabArgs);
private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
