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
		ForestNumZones(FInt32Interval(1, 2)),
		ForestPercentage(FFloatInterval(40.f, 60.f)),
		DesertNumZones(FInt32Interval(2, 3)),
		DesertPercentage(FFloatInterval(10.f, 30.f))
	{}
	

	/**  Mix different trees in the same zone*/
	UPROPERTY(EditAnywhere, Category = NatureSettings, meta = (DisplayName = "Mix Trees"))
	bool bMixTrees;

    /** Number of zones with high density of trees */
	UPROPERTY(EditAnywhere, Category = NatureSettings, meta = (DisplayName = "Forest Zones", UIMin = 0, ClampMin = 0, UIMax = 25, ClampMax = 25))
	FInt32Interval ForestNumZones;

    /** Percentage of total area dedicated to zones with high density of trees */
	UPROPERTY(EditAnywhere, Category = NatureSettings, meta = (DisplayName = "Forest Percent", UIMin = 0, ClampMin = 0, UIMax = 100, ClampMax = 100))
	FFloatInterval ForestPercentage;
    
	/** Number of zones without trees */
	UPROPERTY(EditAnywhere, Category = NatureSettings, meta = (DisplayName = "Desert Zones", UIMin = 0, ClampMin = 0, UIMax = 25, ClampMax = 25))
	FInt32Interval DesertNumZones;

    /** Percentage of total area dedicated to zones without trees */
	UPROPERTY(EditAnywhere, Category = NatureSettings, meta = (DisplayName = "Desert Percent", UIMin = 0, ClampMin = 0, UIMax = 100, ClampMax = 100))
	FFloatInterval DesertPercentage;


	FString ToString()
	{
		return FString(TEXT("Settings - ")) 
			+ FString(TEXT("bMixTrees: ")) + (bMixTrees ? TEXT("True") : TEXT("False"))
			+ FString(TEXT(", ForestNumZones: Min ")) + FString::FromInt(ForestNumZones.Min) + FString(TEXT(", Max ")) + FString::FromInt(ForestNumZones.Max)
			+ FString(TEXT(", ForestPercentage: Min ")) + FString::SanitizeFloat(ForestPercentage.Min) + FString(TEXT(", Max ")) + FString::SanitizeFloat(ForestPercentage.Max)
			+ FString(TEXT(", DesertNumZones: Min ")) + FString::FromInt(DesertNumZones.Min) + FString(TEXT(", Max ")) + FString::FromInt(DesertNumZones.Max)
			+ FString(TEXT(", DesertPercentage: Min ")) + FString::SanitizeFloat(DesertPercentage.Min) + FString(TEXT(", Max ")) + FString::SanitizeFloat(DesertPercentage.Max)
		;
	}
};
