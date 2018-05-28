// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LeCorbusierEdModeToolkit.h"
#include "LeCorbusierEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"


#define LOCTEXT_NAMESPACE "FLeCorbusierEdModeToolkit"

FLeCorbusierEdModeToolkit::FLeCorbusierEdModeToolkit()
{
}

void FLeCorbusierEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() == 1;
		}
	};

	SAssignNew(ToolkitWidget, SBorder)
	[
		SNew(SVerticalBox)

		// SelectedActor is Plugin Land Input
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("HowToEnable", "Select the land to build the environment."))
		]
			
		// Environment Settings
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SAssignNew(EnvironmentSettingsWidget, SEnvironmentSettings)
		]

		// Assets and Details
		+ SVerticalBox::Slot()
		.FillHeight(1)
		.Padding(5)
		[
			SAssignNew(AssetLoaderWidget, SAssetLoader)
		]

		// Build Environment Button
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SNew(SButton)
			.Text(LOCTEXT("BuildButton", "Build Environment"))
			.IsEnabled_Static(&Locals::IsWidgetEnabled)
			.OnClicked(this, &FLeCorbusierEdModeToolkit::DoBuildEnvironment)
		]

	];
		
	FModeToolkit::Init(InitToolkitHost);
}

FName FLeCorbusierEdModeToolkit::GetToolkitFName() const
{
	return FName("LeCorbusierEdMode");
}

FText FLeCorbusierEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("LeCorbusierEdModeToolkit", "DisplayName", "LeCorbusierEdMode Tool");
}

class FEdMode* FLeCorbusierEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FLeCorbusierEdMode::EM_LeCorbusierEdModeId);
}

FReply FLeCorbusierEdModeToolkit::DoBuildEnvironment()
{
	UE_LOG(LogTemp, Warning, TEXT("BUILDING ENVIRONMENT"));
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
