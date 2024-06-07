#include "ManualResetWidget.h"
#include "SlateOptMacros.h"
#include "Name Space/VersioningHelper.h"
#include "Widgets/Input/SNumericEntryBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SManualResetWidget::Construct(FArguments const & InArgs, UVersionAsset * InVersionAsset)
{
	VersionAsset = InVersionAsset;
	OnConfirmed = InArgs._OnConfirmed;
	RequestedMinorVersion = VersioningHelper::GetMinorVersionRef(VersionAsset);
	RequestedPatchVersion = VersioningHelper::GetPatchVersionRef(VersionAsset);
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(SNumericEntryBox<int>)
				.Value(1)
				.IsEnabled(false)
			]
			+ SHorizontalBox::Slot()
			.Padding(FMargin(2, 0))
			[
				SNew(SNumericEntryBox<int>)
				.Value_Lambda([this] { return RequestedMinorVersion; })
				.MinValue(0)
				.OnValueCommitted(this, &SManualResetWidget::UpdateRequestedMinorVersion)
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SNumericEntryBox<int>)
				.Value_Lambda([this] { return RequestedPatchVersion; })
				.MinValue(0)
				.OnValueCommitted(this, &SManualResetWidget::UpdateRequestedPatchVersion)
			]
		]
		+ SVerticalBox::Slot()
		  .AutoHeight()
		  .Padding(FMargin(0, 5, 0, 0))
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
			.Text(FText::FromString("Ok"))
			.OnClicked_Raw(this, &SManualResetWidget::Confirm)
		]
	];
}

FReply SManualResetWidget::Confirm()
{
	VersioningHelper::SetVersions(VersionAsset, RequestedMinorVersion, RequestedPatchVersion);
	OnConfirmed.ExecuteIfBound();
	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
