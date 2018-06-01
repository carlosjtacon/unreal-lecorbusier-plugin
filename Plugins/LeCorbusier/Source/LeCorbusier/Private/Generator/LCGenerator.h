#pragma once

#include "CoreMinimal.h"
#include "EditorModeManager.h"
#include "Editor.h"
#include "Engine/Selection.h"
#include "Editor/EditorEngine.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Engine/TriggerBox.h"

#include "../Types/ULCAsset.h"

class LCGenerator
{
public:

	/** AVAILABLE ALGORITHMS */
	void CreateEnvironmentRandom(TArray<ULCAsset*> Items);
	void CreateEnvironmentNature(TArray<ULCAsset*> Items, bool bMixDifferentTrees);
	void CreateEnvironmentCity(TArray<ULCAsset*> Items, uint32 NumNatureAreas, float NaturePercentage);

private:

	FMath MathAdvanced;
	FGenericPlatformMath MathBasics;

	/** UTILS METHODS */
	FBox GetFloorSurface();
	void PlacingStaticMesh(UStaticMesh* myStaticMesh, FTransform objectTrasform);
	
	/** DEBUG METHODS */
	void PrintDebugAActor(AActor* Actor);
	void PrintDebugUStaticMesh(UStaticMesh* StaticMesh);
};