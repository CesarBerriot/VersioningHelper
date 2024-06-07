#include "VersioningHelperStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FVersioningHelperStyle::StyleInstance = nullptr;

void FVersioningHelperStyle::Initialize()
{
	if(!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FVersioningHelperStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FVersioningHelperStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("VersioningHelperStyle"));
	return StyleSetName;
}

FVector2D const Icon16x16(16.0f, 16.0f);
FVector2D const Icon20x20(20.0f, 20.0f);

TSharedRef<FSlateStyleSet> FVersioningHelperStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("VersioningHelperStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("VersioningHelper")->GetBaseDir() / TEXT("Resources"));

	Style->Set("VersioningHelper.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FVersioningHelperStyle::ReloadTextures()
{
	if(FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

ISlateStyle const & FVersioningHelperStyle::Get()
{
	return *StyleInstance;
}
