// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ULCSettingsCity.generated.h"

UCLASS()
class ULCSettingsCity : public UObject
{
	GENERATED_BODY()

public:

	ULCSettingsCity(): 
		NatureNumAreas(FInt32Interval(1, 3)),
		NaturePercentage(FFloatInterval(10.f, 30.f))
	{}
	

    /** Number of areas dedicated to nature */
	UPROPERTY(EditAnywhere, Category = CitySettings, meta = (DisplayName = "Nature Areas", UIMin = 0, ClampMin = 0, UIMax = 25, ClampMax = 25))
	FInt32Interval NatureNumAreas;

    /** Percentage of total area dedicated to nature areas */
	UPROPERTY(EditAnywhere, Category = CitySettings, meta = (DisplayName = "Nature Percent", UIMin = 0, ClampMin = 0, UIMax = 100, ClampMax = 100))
	FFloatInterval NaturePercentage;


	FString ToString()
	{
		return FString(TEXT("Settings - ")) 
			+ FString(TEXT("NatureNumAreas: Min ")) + FString::FromInt(NatureNumAreas.Min) + FString(TEXT(", Max ")) + FString::FromInt(NatureNumAreas.Max)
			+ FString(TEXT(", NaturePercentage: Min ")) + FString::SanitizeFloat(NaturePercentage.Min) + FString(TEXT(", Max ")) + FString::SanitizeFloat(NaturePercentage.Max)
		;
	}
};
