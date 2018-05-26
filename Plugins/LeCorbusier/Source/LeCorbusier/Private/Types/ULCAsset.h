// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ULCAsset.generated.h"

UENUM()
enum class EAssetsType : uint8
{
	Nature,
	Buildings,
	Vehicles,
	Props
};

UCLASS()
class ULCAsset : public UObject
{
	GENERATED_BODY()

public:
	UObject* Asset;

	// OBJECT TYPE

	/** Type of asset */
	UPROPERTY(EditAnywhere, Category = Type)
	EAssetsType AssetType;


	// GENERAL SETTINGS

	/** Enable this asset */
	UPROPERTY(EditAnywhere, Category = General, meta = (DisplayName = "Enable"))
	uint32 bEnable : 1;

	/** Only one instance */
	UPROPERTY(EditAnywhere, Category = General)
	uint32 Unique : 1;
	
	/** Probability of appearance */
	UPROPERTY(EditAnywhere, Category = General, meta = (DisplayName = "Probability", UIMin = 0, UIMax = 100))
	float Probability;


	// PLACEMENT SETTINGS
	
	/** Minimum distance between instances */
	UPROPERTY(EditAnywhere, Category = Placement, meta = (UIMin = 0, ClampMin = 0))
	float Radius;

	/** Scale adjustment interval */
	UPROPERTY(EditAnywhere, Category = Placement, meta = (UIMin = "0.001"))
	FFloatInterval Scale;

	/** Rotation adjustment interval */
	UPROPERTY(EditAnywhere, Category = Placement, meta = (UIMin = 0, UIMax = 360))
	FFloatInterval Rotation;
	
	/** Translation adjustment interval (X) */
	UPROPERTY(EditAnywhere, Category = Placement, meta = (DisplayName = "Translation X", ClampMin = "0.001", UIMin = "0.001"))
	FFloatInterval TranslationX;

	/** Translation adjustment interval (Y) */
	UPROPERTY(EditAnywhere, Category = Placement, meta = (DisplayName = "Translation Y", ClampMin = "0.001", UIMin = "0.001"))
	FFloatInterval TranslationY;

	/** Translation adjustment interval (Z) */
	UPROPERTY(EditAnywhere, Category = Placement, meta = (DisplayName = "Translation X", ClampMin = "0.001", UIMin = "0.001"))
	FFloatInterval TranslationZ;
	
};
