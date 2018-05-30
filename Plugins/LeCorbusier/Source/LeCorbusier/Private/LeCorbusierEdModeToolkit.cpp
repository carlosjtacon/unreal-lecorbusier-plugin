// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LeCorbusierEdModeToolkit.h"

#define LOCTEXT_NAMESPACE "FLeCorbusierEdModeToolkit"

FLeCorbusierEdModeToolkit::FLeCorbusierEdModeToolkit()
{
}

void FLeCorbusierEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
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
			.IsEnabled(this, &FLeCorbusierEdModeToolkit::BuildButtonEnabled)
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

bool FLeCorbusierEdModeToolkit::BuildButtonEnabled() const
{
	return GEditor->GetSelectedActors()->Num() == 1
		&& AssetLoaderWidget->Items.Num() > 0;
}

FReply FLeCorbusierEdModeToolkit::DoBuildEnvironment()
{
	if (EnvironmentSettingsWidget->EnvironmentTypeComboBoxSelectedItem->Equals(TEXT("Random")))
	{
		LCGen.CreateEnvironmentRandom(AssetLoaderWidget->Items);
	}
	else if (EnvironmentSettingsWidget->EnvironmentTypeComboBoxSelectedItem->Equals(TEXT("City")))
	{
		uint32 NumNatureAreas = EnvironmentSettingsWidget->CityNatureAreasSpinBox->GetValue();
		float NaturePercentage = EnvironmentSettingsWidget->CityNaturePercentageSpinBox->GetValue();
		
		LCGen.CreateEnvironmentCity(AssetLoaderWidget->Items, NumNatureAreas, NaturePercentage);
	}
	else if (EnvironmentSettingsWidget->EnvironmentTypeComboBoxSelectedItem->Equals(TEXT("Nature")))
	{
		bool bMixDifferentTrees = EnvironmentSettingsWidget->NatureMixDifferentTreesCheckBox->IsChecked();
		
		LCGen.CreateEnvironmentNature(AssetLoaderWidget->Items, bMixDifferentTrees);
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
