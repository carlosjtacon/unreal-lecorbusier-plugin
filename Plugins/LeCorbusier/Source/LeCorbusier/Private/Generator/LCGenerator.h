#pragma once

#include "CoreMinimal.h"
#include "EditorModeManager.h"
#include "Editor.h"
#include "Engine/Selection.h"
#include "Editor/EditorEngine.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Engine/TriggerBox.h"

#include "../Types/ULCAsset.h"
#include "../Types/ULCSettingsCity.h"
#include "../Types/ULCSettingsNature.h"
#include "../Types/TLCQuadTree.h"

class LCGenerator
{
public:

	/** AVAILABLE ALGORITHMS */
	void CreateEnvironmentRandom(TArray<ULCAsset*> Items);
	void CreateEnvironmentNature(TArray<ULCAsset*> Items, ULCSettingsNature* Settings);
	void CreateEnvironmentCities(TArray<ULCAsset*> Items, ULCSettingsCity* Settings);

private:

	/** CONSTANT VALUES */
	const uint32 NUM_MAX_FAILURES = 20;

	/** LOCAL TYPES */
	enum class ENatureType
	{
		Forest,
		Normal,
		Desert
	};

	class FNatureZone
	{
	public:

		FNatureZone(float area, ENatureType natureType) :
			Area(area),
			NatureType(natureType)
		{};

		float Area;
		FBox2D Boundary;
		ENatureType NatureType;

		friend bool operator <(const FNatureZone& lh, const FNatureZone& rh)
		{
			return lh.Area > rh.Area;
		}
	};

	/** AUXILIAR METHODS FOR ALGORITHMS */
	TLCQuadTree CreateQuadTreeRandom(FBox2D FloorSurface2D, TArray<ULCAsset*> Items);
	TLCQuadTree CreateQuadTreeNature(FBox2D FloorSurface2D, TArray<ULCAsset*> Items, ULCSettingsNature* Settings);
	TLCQuadTree CreateQuadTreeCities(FBox2D FloorSurface2D, TArray<ULCAsset*> Items, ULCSettingsCity* Settings);

	/** UTILS METHODS */
	FBox GetFloorSurface();
	TArray<FNatureZone> GetZonesBySettings(FBox2D FloorSurface2D, ULCSettingsNature* Settings);
	
	float CrossMultiplication(float a, float b, float c);
	float GetProbabilytyChanged(float Probability, ENatureType NatureType, EAssetType AssetType);
	
	void SubdivideFloorTiles(FBox2D Boundary, TArray<FNatureZone> Zones, TArray<FNatureZone>& FinalZones);
	void PlaceQuadTreeIntoLevel(TLCQuadTree QuadTree, float Height);
	void PlaceItemIntoLevel(ULCAsset* Item, FVector Position, FString Name, FBox2D Boundary);
	
	/** DEBUG METHODS */
	void PrintDebugAActor(AActor* Actor);
	void PrintDebugUStaticMesh(UStaticMesh* StaticMesh);
	void PrintDebugFNatureZoneArray(TArray<FNatureZone> NatureZones);
};