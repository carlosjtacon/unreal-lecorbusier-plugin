// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ULCSettingsNature.generated.h"

UCLASS()
class ULCSettingsNature : public UObject
{
	GENERATED_BODY()

public:

	ULCSettingsNature(): 
		bMixTrees(true),
		ForestNumAreas(FInt32Interval(1, 2)),
		ForestPercentage(FFloatInterval(40.f, 60.f)),
		DesertNumAreas(FInt32Interval(2, 3)),
		DesertPercentage(FFloatInterval(10.f, 30.f))
	{}
	

	/**  Mix different trees in the same area*/
	UPROPERTY(EditAnywhere, Category = NatureSettings, meta = (DisplayName = "Mix Trees"))
	bool bMixTrees;

    /** Number of areas with high density of trees */
	UPROPERTY(EditAnywhere, Category = NatureSettings, meta = (DisplayName = "Forest Areas", UIMin = 0, ClampMin = 0, UIMax = 25, ClampMax = 25))
	FInt32Interval ForestNumAreas;

    /** Percentage of total area dedicated to areas with high density of trees */
	UPROPERTY(EditAnywhere, Category = NatureSettings, meta = (DisplayName = "Forest Percent", UIMin = 0, ClampMin = 0, UIMax = 100, ClampMax = 100))
	FFloatInterval ForestPercentage;
    
	/** Number of areas without trees */
	UPROPERTY(EditAnywhere, Category = NatureSettings, meta = (DisplayName = "Desert Areas", UIMin = 0, ClampMin = 0, UIMax = 25, ClampMax = 25))
	FInt32Interval DesertNumAreas;

    /** Percentage of total area dedicated to areas without trees */
	UPROPERTY(EditAnywhere, Category = NatureSettings, meta = (DisplayName = "Desert Percent", UIMin = 0, ClampMin = 0, UIMax = 100, ClampMax = 100))
	FFloatInterval DesertPercentage;


	FString ToString()
	{
		return FString(TEXT("Settings - ")) 
			+ FString(TEXT("bMixTrees: ")) + (bMixTrees ? TEXT("True") : TEXT("False"))
			+ FString(TEXT(", ForestNumAreas: Min ")) + FString::FromInt(ForestNumAreas.Min) + FString(TEXT(", Max ")) + FString::FromInt(ForestNumAreas.Max)
			+ FString(TEXT(", ForestPercentage: Min ")) + FString::SanitizeFloat(ForestPercentage.Min) + FString(TEXT(", Max ")) + FString::SanitizeFloat(ForestPercentage.Max)
			+ FString(TEXT(", DesertNumAreas: Min ")) + FString::FromInt(DesertNumAreas.Min) + FString(TEXT(", Max ")) + FString::FromInt(DesertNumAreas.Max)
			+ FString(TEXT(", DesertPercentage: Min ")) + FString::SanitizeFloat(DesertPercentage.Min) + FString(TEXT(", Max ")) + FString::SanitizeFloat(DesertPercentage.Max)
		;
	}
};
