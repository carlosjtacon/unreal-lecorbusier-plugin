// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LeCorbusierEdModeToolkit.h"
#include "LeCorbusierEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"

#include "Widgets/SEnvironmentSettings.h"
#include "Widgets/SAssetLoader.h"
#include "Widgets/SBuildButton.h"

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
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
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
				SNew(SEnvironmentSettings)
			]

			// Assets and Details
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(5)
			[
				SNew(SAssetLoader)
			]

			// Build Environment Button
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(5)
			[
				SNew(SBuildButton)
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

#undef LOCTEXT_NAMESPACE
