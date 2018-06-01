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
		ScaleXYZ(FFloatInterval(1,1)),
		RotationZ(FFloatInterval(0, 0)),
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
	UPROPERTY(EditAnywhere, Category = Instance, meta = (DisplayName = "Enable"))
	bool bEnable;

	/** Maximum number of instances generated. Zero is unlimited! */
	UPROPERTY(EditAnywhere, Category = Instance, meta = (DisplayName = "Max. Instances", ClampMin = 0))
	uint32 MaxInstances;
	
	/** Probability of appearance */
	UPROPERTY(EditAnywhere, Category = Instance, meta = (DisplayName = "Probability", ClampMin = 0, ClampMax = 1))
	float Probability;

	/** Minimum distance between instances */
	UPROPERTY(EditAnywhere, Category = Instance , meta = (DisplayName = "Radius", ClampMin = 0))
	float Radius;


	// TRANSFORMATION SETTINGS

	/** Scale adjustment interval (XYZ) */
	UPROPERTY(EditAnywhere, Category = Transformation, meta = (DisplayName = "Scale"))
	FFloatInterval ScaleXYZ;

	/** Rotation adjustment interval (Z) */
	UPROPERTY(EditAnywhere, Category = Transformation, meta = (DisplayName = "Rotation"))
	FFloatInterval RotationZ;
	
	/** Translation offset interval (X) */
	UPROPERTY(EditAnywhere, Category = Transformation, meta = (DisplayName = "Translation X"))
	FFloatInterval TranslationX;

	/** Translation offset interval (Y) */
	UPROPERTY(EditAnywhere, Category = Transformation, meta = (DisplayName = "Translation Y"))
	FFloatInterval TranslationY;

	/** Translation offset interval (Z) */
	UPROPERTY(EditAnywhere, Category = Transformation, meta = (DisplayName = "Translation Z"))
	FFloatInterval TranslationZ;
	
};
