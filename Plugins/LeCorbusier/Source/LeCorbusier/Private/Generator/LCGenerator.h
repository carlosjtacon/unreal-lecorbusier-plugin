#pragma once

#include "CoreMinimal.h"
#include "EditorModeManager.h"
#include "Editor.h"
#include "Engine/Selection.h"
#include "Editor/EditorEngine.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Engine/TriggerBox.h"

#include "../Types/ULCAsset.h"
#include "../Types/TLCQuadTree.h"

class LCGenerator
{
public:

	/** AVAILABLE ALGORITHMS */
	void CreateEnvironmentRandom(TArray<ULCAsset*> Items);
	void CreateEnvironmentNature(TArray<ULCAsset*> Items, bool bMixDifferentTrees);
	void CreateEnvironmentCities(TArray<ULCAsset*> Items, uint32 NumNatureAreas, float NaturePercentage);

private:

	/** CONSTANT VALUES */
	const uint32 NUM_MAX_FAILURES = 20;

	/** AUXILIAR METHODS FOR ALGORITHMS */
	TLCQuadTree CreateQuadTreeRandom(FBox2D FloorSurface2D, TArray<ULCAsset*> Items);
	TLCQuadTree CreateQuadTreeNature(FBox2D FloorSurface2D, TArray<ULCAsset*> Items, bool bMixDifferentTrees);
	TLCQuadTree CreateQuadTreeCities(FBox2D FloorSurface2D, TArray<ULCAsset*> Items, uint32 NumNatureAreas, float NaturePercentage);

	/** UTILS METHODS */
	FBox GetFloorSurface();
	void PlaceQuadTreeIntoLevel(TLCQuadTree QuadTree, float Height);
	void PlaceItemIntoLevel(ULCAsset* Item, FVector Position, FString Name, FBox2D Boundary);
	
	/** DEBUG METHODS */
	void PrintDebugAActor(AActor* Actor);
	void PrintDebugUStaticMesh(UStaticMesh* StaticMesh);
};