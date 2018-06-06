// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "SlateOptMacros.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"

#include "../Types/ULCSettingsNature.h"
#include "../Types/ULCSettingsCity.h"

/**
 * 
 */
class SEnvironmentSettings : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SEnvironmentSettings)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// Environment type selected
	TSharedPtr<FString> EnvironmentTypeComboBoxSelectedItem;

	// Environment Settings
	ULCSettingsNature* SettingsNature;
	ULCSettingsCity* SettingsCity;

private:

	// Environment type selector combo box and items
	TSharedPtr<STextComboBox> EnvironmentTypeComboBox;
	TArray<TSharedPtr<FString>> EnvironmentTypeComboBoxItems;

	// Callback for selection changes in the environment combo box
	void HandleTextComboBoxSelectionChanged (TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);

	// Settings details view
	TSharedPtr<class IDetailsView> SettingsNatureWidget;
	TSharedPtr<class IDetailsView> SettingsCityWidget;

	// Visibility delegates for type-specific settings
	EVisibility GetRandomVisibility() const;
	EVisibility GetCityVisibility() const;
	EVisibility GetNatureVisibility() const;
};
