#include "MainWidget.h"
#include "SlateOptMacros.h"
#include "Name Space/VersioningHelper.h"
#include "Slate Widgets/Manual Reset Popup/ManualResetWidget.h"
#include "Widgets/Layout/SScaleBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMainWidget::Construct(FArguments const & InArgs)
{
	VersionAsset = VersioningHelper::GetOrCreateVersionAsset();
	ChildSlot
	[
		VersionAsset ? ConstructNormalInterface() : ConstructErrorInterface()
	];
}

TSharedRef<SWidget> SMainWidget::ConstructErrorInterface()
{
	return SNew(SBox)
		   .HAlign(HAlign_Center)
		   .VAlign(VAlign_Center)
	       [
		       SNew(STextBlock)
		       .Text(FText::FromString("An error occured. Read engine notifications for more info."))
	       ];
}

TSharedRef<SWidget> SMainWidget::ConstructNormalInterface()
{
	FMargin ButtonsPadding(0, 0, 7, 0);
	return SNew(SBox)
		   .HAlign(HAlign_Center)
		   .VAlign(VAlign_Center)
	       [
		       SNew(SVerticalBox)
		       + SVerticalBox::Slot()
		       .AutoHeight()
		       [
			       SNew(SBox)
			       .MinDesiredHeight(130)
			       [
				       SNew(SScaleBox)
				       .Stretch(EStretch::ScaleToFit)
				       [
					       SNew(STextBlock)
					       .Text_Lambda([this] { return FText::FromString(MakeVersionString()); })
				       ]
			       ]
		       ]
		       + SVerticalBox::Slot()
		       .AutoHeight()
		       .HAlign(HAlign_Center)
		       [
			       SNew(SHorizontalBox)
			       + SHorizontalBox::Slot()
			         .AutoWidth()
			         .Padding(ButtonsPadding)
			       [
				       SNew(SButton)
				       .Text(FText::FromString("Increase Minor Version"))
				       .OnClicked_Raw(this, &SMainWidget::IncreaseMinorVersion)
			       ]
			       + SHorizontalBox::Slot()
			       .AutoWidth()
			       [
				       SNew(SButton)
				       .Text(FText::FromString("Increase Patch Version"))
				       .OnClicked_Raw(this, &SMainWidget::IncreasePatchVersion)
			       ]
		       ]
		       + SVerticalBox::Slot()
		         .Padding(FMargin(0, 7, 0, 0))
		         .AutoHeight()
		         .HAlign(HAlign_Center)
		       [
			       SNew(SHorizontalBox)
			       + SHorizontalBox::Slot()
			         .AutoWidth()
			         .Padding(ButtonsPadding)
			       [
				       SNew(SButton)
				       .Text(FText::FromString("Manual Reset"))
				       .OnClicked_Raw(this, &SMainWidget::CreateManualResetPopup)
			       ]
			       + SHorizontalBox::Slot()
			       .AutoWidth()
			       [
				       SNew(SButton)
		    		   .Text(FText::FromString("Save"))
		    		   .IsEnabled_Lambda([this] { return VersionAsset->GetPackage()->IsDirty(); })
				       .OnClicked_Raw(this, &SMainWidget::SaveVersionAsset)
			       ]
		       ]
	       ];
}

TSharedRef<SWindow> SMainWidget::ConstructManualResetPopupWindow()
{
	return SAssignNew(ManualResetPopupWindow, SWindow)
		   .Title(FText::FromString("Manual Reset"))
		   .ClientSize(FVector2D(350, 52))
		   .Content()
	       [
		       SNew(SManualResetWidget, VersionAsset)
		       .OnConfirmed_Lambda([this]{ FSlateApplication::Get().RequestDestroyWindow(ManualResetPopupWindow.ToSharedRef()); })
	       ];
}

FReply SMainWidget::IncreaseMinorVersion()
{
	VersioningHelper::IncreaseMinorVersion(VersionAsset);
	return FReply::Handled();
}

FReply SMainWidget::IncreasePatchVersion()
{
	VersioningHelper::IncreasePatchVersion(VersionAsset);
	return FReply::Handled();
}

FString SMainWidget::MakeVersionString()
{
	int & MinorVersion = VersioningHelper::GetMinorVersionRef(VersionAsset);
	int & PatchVersion = VersioningHelper::GetPatchVersionRef(VersionAsset);
	return FString::Printf(L"1.%i.%i", MinorVersion, PatchVersion);
}

FReply SMainWidget::SaveVersionAsset()
{
	VersioningHelper::SaveVersionAsset(VersionAsset);
	return FReply::Handled();
}

FReply SMainWidget::CreateManualResetPopup()
{
	FSlateApplication::Get().AddModalWindow(ConstructManualResetPopupWindow(), this->AsShared());
	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
