// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ULCAsset.generated.h"

UENUM()
enum class EAssetType : uint8
{
	/** Nature related assets */
	Tree, Bush, Rock, Cabin, Ruins,
	
	/** City related assets */
	Building, Monument, Streetlight, Vehicle, ThrashBin, Dumpster, Billboard
};

UCLASS()
class ULCAsset : public UObject
{
	GENERATED_BODY()

public:

	ULCAsset(): 
		bEnable(true),
		MaxInstances(0),
		Probability(0.7f),
		Radius(0),
		Scale(FFloatInterval(1,1)),
		Rotation(FFloatInterval(0, 0)),
		TranslationX(FFloatInterval(0, 0)),
		TranslationY(FFloatInterval(0, 0)),
		TranslationZ(FFloatInterval(0, 0))
	{}
	
	UObject* Asset;

	// OBJECT TYPE

	/** Type of asset */
	UPROPERTY(EditAnywhere, Category = Type, meta = (DisplayName = "Type"))
	EAssetType AssetType;


	// GENERAL SETTINGS

	/** Enable this asset */
	UPROPERTY(EditAnywhere, Category = General, meta = (DisplayName = "Enable"))
	bool bEnable;

	/** Maximum number of instances generated. Zero is unlimited! */
	UPROPERTY(EditAnywhere, Category = General, meta = (DisplayName = "Max. Instances", ClampMin = 0))
	uint32 MaxInstances;
	
	/** Probability of appearance */
	UPROPERTY(EditAnywhere, Category = General, meta = (DisplayName = "Probability", ClampMin = 0, ClampMax = 1))
	float Probability;


	// PLACEMENT SETTINGS
	
	/** Minimum distance between instances */
	UPROPERTY(EditAnywhere, Category = Placement, meta = (DisplayName = "Radius", ClampMin = 0))
	float Radius;

	/** Scale adjustment interval */
	UPROPERTY(EditAnywhere, Category = Placement, meta = (DisplayName = "Scale"))
	FFloatInterval Scale;

	/** Rotation adjustment interval */
	UPROPERTY(EditAnywhere, Category = Placement, meta = (DisplayName = "Rotation"))
	FFloatInterval Rotation;
	
	/** Translation offset interval (X) */
	UPROPERTY(EditAnywhere, Category = Placement, meta = (DisplayName = "Translation X"))
	FFloatInterval TranslationX;

	/** Translation offset interval (Y) */
	UPROPERTY(EditAnywhere, Category = Placement, meta = (DisplayName = "Translation Y"))
	FFloatInterval TranslationY;

	/** Translation offset interval (Z) */
	UPROPERTY(EditAnywhere, Category = Placement, meta = (DisplayName = "Translation X"))
	FFloatInterval TranslationZ;
	
};
