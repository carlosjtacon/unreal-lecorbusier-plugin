#pragma once

#include "CoreMinimal.h"
#include "EditorModeManager.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"

#include "../Types/ULCAsset.h"

class LCGenerator
{
public:

	void CreateEnvironmentRandom(TArray<ULCAsset*> Items);
	void CreateEnvironmentNature(TArray<ULCAsset*> Items, bool bMixDifferentTrees);
	void CreateEnvironmentCity(TArray<ULCAsset*> Items, uint32 NumNatureAreas, float NaturePercentage);

private:

	void PlacingStaticMesh(UStaticMesh* myStaticMesh);
};