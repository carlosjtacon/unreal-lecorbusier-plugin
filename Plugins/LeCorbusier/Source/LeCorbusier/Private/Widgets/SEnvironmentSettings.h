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

private:
	// Holds the combo box in the STextComboBox example.
	TSharedPtr<STextComboBox> EnvironmentTypeComboBox;
	
	// Holds the check box for mixing different kind of trees
	TSharedPtr<SCheckBox> NatureMixDifferentTreesCheckBox;

	// Holds the spin box for number of nature areas inside the city
	TSharedPtr<SSpinBox<uint32>> CityNatureAreasSpinBox;
	
	// Holds the spin box for the percentage of city used for nature areas
	TSharedPtr<SSpinBox<float>> CityNaturePercentageSpinBox;

	// Holds the selected item of the text combo box in the STextComboBox example.
	TSharedPtr<FString> EnvironmentTypeComboBoxSelectedItem;

	// Holds the text options for the STextComboBox example.
	TArray<TSharedPtr<FString> > EnvironmentTypeComboBoxItems;

	// Callback for selection changes in the STextComboBox example.
	void HandleTextComboBoxSelectionChanged (TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);

	// Visibility delegates for type-specific settings
	EVisibility GetRandomVisibility() const;
	EVisibility GetCityVisibility() const;
	EVisibility GetNatureVisibility() const;

};
