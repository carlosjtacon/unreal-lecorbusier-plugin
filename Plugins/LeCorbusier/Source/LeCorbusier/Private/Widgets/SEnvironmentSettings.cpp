// Fill out your copyright notice in the Description page of Project Settings.

#include "SEnvironmentSettings.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "FSEnvironmentSettings"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SEnvironmentSettings::Construct(const FArguments& InArgs)
{
	{
		EnvironmentTypeComboBoxSelectedItem = MakeShareable(new FString(TEXT("Random")));
		
		EnvironmentTypeComboBoxItems.Add(EnvironmentTypeComboBoxSelectedItem);
		EnvironmentTypeComboBoxItems.Add(MakeShareable(new FString(TEXT("City"))));
		EnvironmentTypeComboBoxItems.Add(MakeShareable(new FString(TEXT("Nature"))));
	}

	ChildSlot
	[
		SNew(SVerticalBox)

		// Header
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SHeader)
			.Content()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("EnvironmentSettings", "Environment Settings"))
			]
		]

		// Environment Type
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SHorizontalBox)
			
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("EnvironmentType", "Environment Type"))
			]
			
			+ SHorizontalBox::Slot()
			[
				SAssignNew(EnvironmentTypeComboBox, STextComboBox)
				.OptionsSource(&EnvironmentTypeComboBoxItems)
				.InitiallySelectedItem(EnvironmentTypeComboBoxSelectedItem)	
				.OnSelectionChanged(this, &SEnvironmentSettings::HandleTextComboBoxSelectionChanged)
			]
		]

		// Type Specific Settings
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SBorder)
			[
				SNew(SOverlay)

				// Random
				+ SOverlay::Slot()
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(5)
					[
						SNew(STextBlock)
						.Visibility(this, &SEnvironmentSettings::GetRandomVisibility)
						.Text(LOCTEXT("RandomSettings", "Random environment has no specific settings."))
					]
				]

				// City
				+ SOverlay::Slot()
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(5)
					[
						SNew(STextBlock)
						.Visibility(this, &SEnvironmentSettings::GetCityVisibility)
						.Text(LOCTEXT("CitySettings", "City Settings"))
					]
				]
				
				// Nature
				+ SOverlay::Slot()
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(5)
					[
						SNew(STextBlock)
						.Visibility(this, &SEnvironmentSettings::GetNatureVisibility)
						.Text(LOCTEXT("NatureSettings", "Nature Settings"))
					]
				]
			]
		]

	];
	
}

void SEnvironmentSettings::HandleTextComboBoxSelectionChanged (TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	EnvironmentTypeComboBoxSelectedItem = NewSelection;
}

EVisibility SEnvironmentSettings::GetRandomVisibility() const
{
	return EnvironmentTypeComboBoxSelectedItem->Equals(TEXT("Random")) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility SEnvironmentSettings::GetCityVisibility() const
{
	return EnvironmentTypeComboBoxSelectedItem->Equals(TEXT("City")) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility SEnvironmentSettings::GetNatureVisibility() const
{
	return EnvironmentTypeComboBoxSelectedItem->Equals(TEXT("Nature")) ? EVisibility::Visible : EVisibility::Collapsed;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE