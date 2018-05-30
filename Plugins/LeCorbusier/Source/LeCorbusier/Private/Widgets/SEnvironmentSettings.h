// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "SlateOptMacros.h"

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
	
	// Nature settings widget: mix different kind of trees
	TSharedPtr<SCheckBox> NatureMixDifferentTreesCheckBox;

	// City settings widget: number of nature areas inside the city
	TSharedPtr<SSpinBox<uint32>> CityNatureAreasSpinBox;

	// City settings widget: percentage of city used for nature areas
	TSharedPtr<SSpinBox<float>> CityNaturePercentageSpinBox;

private:

	// Environment type selector combo box and items
	TSharedPtr<STextComboBox> EnvironmentTypeComboBox;
	TArray<TSharedPtr<FString>> EnvironmentTypeComboBoxItems;

	// Callback for selection changes in the environment combo box
	void HandleTextComboBoxSelectionChanged (TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);

	// Visibility delegates for type-specific settings
	EVisibility GetRandomVisibility() const;
	EVisibility GetCityVisibility() const;
	EVisibility GetNatureVisibility() const;
};
