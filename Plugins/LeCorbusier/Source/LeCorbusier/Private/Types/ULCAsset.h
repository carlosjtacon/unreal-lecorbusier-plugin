// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ULCAsset.generated.h"

/**
 * 
 */
UCLASS()
class ULCAsset : public UObject
{
	GENERATED_BODY()

public:
	UObject* Asset;
	
	UPROPERTY(EditAnywhere, Category = General, meta = (DisplayName = "Density", UIMin = 0, ClampMin = 0))
	float Density;
	
};
