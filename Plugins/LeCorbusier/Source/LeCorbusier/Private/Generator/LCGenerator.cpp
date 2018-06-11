#include "LCGenerator.h"

#define LOCTEXT_NAMESPACE "LCGeneratorNamespace"


/** AVAILABLE ALGORITHMS */
void LCGenerator::CreateEnvironmentRandom(TArray<ULCAsset*> Items)
{
	UE_LOG(LogTemp, Warning, TEXT("BUILDING ENVIRONMENT RANDOM"));
	// Let editor know that we're about to do something that we want to undo/redo
	GEditor->BeginTransaction(LOCTEXT("CreateEnvironmentRandomTransaction", "Generate Environment"));
	
	// Get surface coordinates & 2D 
	FBox FloorSurface = GetFloorSurface(); // UE_LOG(LogTemp, Warning, TEXT("FloorSurface is %s"), *FloorSurface.ToString());
	FBox2D FloorSurface2D(FVector2D(FloorSurface.Min.X, FloorSurface.Min.Y), FVector2D(FloorSurface.Max.X, FloorSurface.Max.Y));

	// Generate random environment
	TLCQuadTree QuadTree = CreateQuadTreeRandom(FloorSurface2D, Items);
	PlaceQuadTreeIntoLevel(QuadTree, FloorSurface.Min.Z);

	// We're done generating the environment so we close the transaction
	GEditor->EndTransaction();
}

void LCGenerator::CreateEnvironmentNature(TArray<ULCAsset*> Items, ULCSettingsNature* Settings)
{
	UE_LOG(LogTemp, Warning, TEXT("BUILDING ENVIRONMENT NATURE (%s)"), *Settings->ToString());
	// Let editor know that we're about to do something that we want to undo/redo
	GEditor->BeginTransaction(LOCTEXT("CreateEnvironmentRandomTransaction", "Generate Environment"));

	// Get surface coordinates & 2D 
	FBox FloorSurface = GetFloorSurface(); // UE_LOG(LogTemp, Warning, TEXT("FloorSurface is %s"), *FloorSurface.ToString());
	FBox2D FloorSurface2D(FVector2D(FloorSurface.Min.X, FloorSurface.Min.Y), FVector2D(FloorSurface.Max.X, FloorSurface.Max.Y));

	// Generate random environment
	TLCQuadTree QuadTree = CreateQuadTreeNature(FloorSurface2D, Items, Settings);
	PlaceQuadTreeIntoLevel(QuadTree, FloorSurface.Min.Z);
	
	// We're done generating the environment so we close the transaction
	GEditor->EndTransaction();
}

void LCGenerator::CreateEnvironmentCities(TArray<ULCAsset*> Items, ULCSettingsCity* Settings)
{
	UE_LOG(LogTemp, Warning, TEXT("BUILDING ENVIRONMENT CITY (%s)"), *Settings->ToString());
	// Let editor know that we're about to do something that we want to undo/redo
	GEditor->BeginTransaction(LOCTEXT("CreateEnvironmentRandomTransaction", "Generate Environment"));

	// Get surface coordinates & 2D 
	FBox FloorSurface = GetFloorSurface(); // UE_LOG(LogTemp, Warning, TEXT("FloorSurface is %s"), *FloorSurface.ToString());
	FBox2D FloorSurface2D(FVector2D(FloorSurface.Min.X, FloorSurface.Min.Y), FVector2D(FloorSurface.Max.X, FloorSurface.Max.Y));

	// Generate random environment
	TLCQuadTree QuadTree = CreateQuadTreeCities(FloorSurface2D, Items, Settings);
	PlaceQuadTreeIntoLevel(QuadTree, FloorSurface.Min.Z);

	// We're done generating the environment so we close the transaction
	GEditor->EndTransaction();
}


/** AUXILIAR METHODS FOR ALGORITHMS */
TLCQuadTree LCGenerator::CreateQuadTreeRandom(FBox2D FloorSurface2D, TArray<ULCAsset*> Items)
{
	// Initialize failures count and the QuadTree
	uint32 NumFailures = 0; TLCQuadTree QuadTree(FloorSurface2D, 4);
	// Initialize a list to count the current instances of each item in items list
	TArray<uint32> CurrentItemInstances; for (int i = 0; i < Items.Num(); i++) CurrentItemInstances.Add(0);
	
	while (NumFailures < NUM_MAX_FAILURES) {
		// Pick a random item and check enabled and max instances
		int32 RandomItemNum = FMath::RandRange(0, Items.Num() - 1);
		ULCAsset* Item = Items[RandomItemNum];
		bool bInstancesAvailable = (Item->MaxInstances == 0 || Item->MaxInstances > CurrentItemInstances[RandomItemNum]);

		TArray<TLCParticle> CollisionParticles;
		if (Item->bEnable && bInstancesAvailable)
		{
			// Generate random position within boundary and check collisions in radius
			FVector2D RandomPosition(FMath::FRandRange(FloorSurface2D.Min.X, FloorSurface2D.Max.X), FMath::FRandRange(FloorSurface2D.Min.Y, FloorSurface2D.Max.Y));
			TLCParticle Particle(RandomPosition, Item->Radius); Particle.Item = Item;

			QuadTree.Query(Particle, CollisionParticles);
			bool ProbabilitySuccess = (Item->Probability > FGenericPlatformMath::FRand());

			// Insert only if no collisions found and probability
			if (CollisionParticles.Num() == 0 && ProbabilitySuccess)
			{
				QuadTree.Insert(Particle);
				if (NumFailures > 0) NumFailures--;
				CurrentItemInstances[RandomItemNum]++;
				continue;
			}
		}

		// Increase number of failures only if collision or max instances generated
		// because the rest of checks doesn't mean that the environment is finished
		if (!bInstancesAvailable || CollisionParticles.Num() != 0) NumFailures++;
	}

	return QuadTree;
}

TLCQuadTree LCGenerator::CreateQuadTreeNature(FBox2D FloorSurface2D, TArray<ULCAsset*> Items, ULCSettingsNature* Settings)
{
	// bool bMix = Settings->bMixTrees;
	TArray<FNatureZone> NatureZonesWithBoundaries;
	TArray<FNatureZone> NatureZonesByPercentage = GetZonesBySettings(FloorSurface2D, Settings);
	SubdivideFloorTiles(FloorSurface2D, NatureZonesByPercentage, NatureZonesWithBoundaries); // PrintDebugFNatureZoneArray(NatureZonesWithBoundaries);
	
	TLCQuadTree QuadTree(FloorSurface2D, 4);
	// Initialize a list to count the current instances of each item in items list
	TArray<uint32> CurrentItemInstances; for (int i = 0; i < Items.Num(); i++) CurrentItemInstances.Add(0);
	for (int n = 0; n < NatureZonesWithBoundaries.Num(); n++)
	{
		FNatureZone NatureZone = NatureZonesWithBoundaries[n];
		
		// Initialize failures count
		uint32 NumFailures = 0; 

		while (NumFailures < NUM_MAX_FAILURES) {
			// Pick a random item and check enabled and max instances
			int32 RandomItemNum = FMath::RandRange(0, Items.Num() - 1);
			ULCAsset* Item = Items[RandomItemNum];
			bool bInstancesAvailable = (Item->MaxInstances == 0 || Item->MaxInstances > CurrentItemInstances[RandomItemNum]);

			TArray<TLCParticle> CollisionParticles;
			if (Item->bEnable && bInstancesAvailable)
			{
				// Generate random position within boundary of NatureZone and check collisions in radius
				FVector2D RandomPosition(FMath::FRandRange(NatureZone.Boundary.Min.X, NatureZone.Boundary.Max.X), FMath::FRandRange(NatureZone.Boundary.Min.Y, NatureZone.Boundary.Max.Y));
				TLCParticle Particle(RandomPosition, Item->Radius); Particle.Item = Item;

				// Check probability before querying the QuadTree
				bool ProbabilitySuccess = (GetProbabilytyChanged(Item->Probability, NatureZone.NatureType, Item->AssetType) > FGenericPlatformMath::FRand());
				if (!ProbabilitySuccess) continue;

				// Insert only if no collisions found
				QuadTree.Query(Particle, CollisionParticles);
				if (CollisionParticles.Num() == 0)
				{
					QuadTree.Insert(Particle);
					if (NumFailures > 0) NumFailures--;
					CurrentItemInstances[RandomItemNum]++;
					continue;
				}
			}

			// Increase number of failures only if collision or max instances generated
			// because the rest of checks doesn't mean that the environment is finished
			if (!bInstancesAvailable || CollisionParticles.Num() != 0) NumFailures++;
		}
	}

	return QuadTree;
}

TLCQuadTree LCGenerator::CreateQuadTreeCities(FBox2D FloorSurface2D, TArray<ULCAsset*> Items, ULCSettingsCity* Settings)
{
	TLCQuadTree QuadTree(FloorSurface2D, 4);

	//todo: generate city area

	return QuadTree;
}


/** UTILS METHODS */
FBox LCGenerator::GetFloorSurface()
{
	// Get selected actor as surface 
	TArray<UObject*> SelectedActors;
	GEditor->GetSelectedActors()->GetSelectedObjects(AActor::StaticClass(), SelectedActors);
	AActor* SelectedActor = Cast<AActor>(SelectedActors[0]); // PrintDebugAActor(SelectedActor);
	
	FVector MinPoint, MaxPoint;
	if (SelectedActor->GetClass()->IsChildOf(AStaticMeshActor::StaticClass()))
	{
		// UE_LOG(LogTemp, Error, TEXT("IT IS A STATIC MESH"));
		MinPoint = SelectedActor->GetComponentsBoundingBox().Min;
		MaxPoint = SelectedActor->GetComponentsBoundingBox().Max;
		MinPoint.Z = SelectedActor->GetComponentsBoundingBox().Max.Z;
	}
	else if (SelectedActor->GetClass()->IsChildOf(ATriggerBox::StaticClass()))
	{
		// UE_LOG(LogTemp, Error, TEXT("IT IS A TRIGGER BOX"));
		MinPoint = SelectedActor->GetComponentsBoundingBox().Min;
		MaxPoint = SelectedActor->GetComponentsBoundingBox().Max;
		MaxPoint.Z = SelectedActor->GetComponentsBoundingBox().Min.Z;
	}

	FBox FloorSurface(MinPoint, MaxPoint);
	return FloorSurface;
}

float LCGenerator::CrossMultiplication(float a, float b, float c)
{
	return b * c / a;
}

float LCGenerator::GetProbabilytyChanged(float Probability, ENatureType NatureType, EAssetType AssetType)
{
	// In forests we have trees and rocks and no bushes
	bool bForestGoodTypes = (AssetType == EAssetType::Tree || AssetType == EAssetType::Rock);
	if (NatureType == ENatureType::Forest && bForestGoodTypes) return 1.f;

	bool bForestBaaadTypes = (AssetType == EAssetType::Cabin || AssetType == EAssetType::Ruins || AssetType == EAssetType::Bush);
	if (NatureType == ENatureType::Forest && bForestBaaadTypes) return 0.f;
	
	// In desert areas we have rocks and bushes and no trees
	bool bDesertGoodTypes = (AssetType == EAssetType::Bush || AssetType == EAssetType::Rock);
	if (NatureType == ENatureType::Desert && bDesertGoodTypes) return 1.f;

	bool bDesertBaaadTypes = (AssetType == EAssetType::Tree);
	if (NatureType == ENatureType::Desert && bDesertBaaadTypes) return 0.f;


	// No changes in probability
	return Probability;
}

TArray<LCGenerator::FNatureZone> LCGenerator::GetZonesBySettings(FBox2D FloorSurface2D, ULCSettingsNature* Settings)
{
	// Calculate number of areas of each type
	int32 ForestNumZones = FMath::RandRange(Settings->ForestNumZones.Min, Settings->ForestNumZones.Max);
	int32 DesertNumZones = FMath::RandRange(Settings->DesertNumZones.Min, Settings->DesertNumZones.Max);
	int32 NormalNumZones = FMath::CeilToInt((ForestNumZones + DesertNumZones) / 2);

	// Calculate area related to percentage
	float ForestPercentage = FMath::FRandRange(Settings->ForestPercentage.Min, Settings->ForestPercentage.Max);
	float DesertPercentage = FMath::FRandRange(Settings->DesertPercentage.Min, Settings->DesertPercentage.Max);
	float ForestArea = CrossMultiplication(100.f, FloorSurface2D.GetArea(), ForestPercentage);
	float DesertArea = CrossMultiplication(100.f, FloorSurface2D.GetArea(), DesertPercentage);
	float NormalArea = FloorSurface2D.GetArea() - ForestArea - DesertArea;

	// Check if total area is grater than 100%  ¯\_(ツ)_/¯
	if (ForestArea + DesertArea > FloorSurface2D.GetArea())
	{
		ForestArea = CrossMultiplication(ForestArea + DesertArea, FloorSurface2D.GetArea(), ForestArea);
		DesertArea = CrossMultiplication(ForestArea + DesertArea, FloorSurface2D.GetArea(), DesertArea);
		NormalArea = FloorSurface2D.GetArea() - ForestArea - DesertArea;
	}

	// Split areas into zones
	int32 RemainingZones;
	float AreaTo, AreaFrom;
	TArray<FNatureZone> NatureZones;

	if (ForestArea != 0 && ForestNumZones != 0)
	{
		AreaFrom = 0;
		RemainingZones = ForestNumZones;
		for (int i = 0; i < ForestNumZones; i++)
		{
			if (RemainingZones == 1) AreaTo = ForestArea;
			else AreaTo = FMath::FRandRange(AreaFrom, ForestArea);

			NatureZones.Add(FNatureZone(AreaTo - AreaFrom, ENatureType::Forest));
			AreaFrom = AreaTo;
			RemainingZones--;
		}
	}
	
	if (DesertArea != 0 && DesertNumZones != 0)
	{
		AreaFrom = 0;
		RemainingZones = DesertNumZones;
		for (int i = 0; i < DesertNumZones; i++)
		{
			if (RemainingZones == 1) AreaTo = DesertArea;
			else AreaTo = FMath::FRandRange(AreaFrom, DesertArea);

			NatureZones.Add(FNatureZone(AreaTo - AreaFrom, ENatureType::Desert));
			AreaFrom = AreaTo;
			RemainingZones--;
		}
	}

	if (NormalArea != 0 && NormalNumZones != 0)
	{
		AreaFrom = 0;
		RemainingZones = NormalNumZones;
		for (int i = 0; i < NormalNumZones; i++)
		{
			if (RemainingZones == 1) AreaTo = NormalArea;
			else AreaTo = FMath::FRandRange(AreaFrom, NormalArea);

			NatureZones.Add(FNatureZone(AreaTo - AreaFrom, ENatureType::Normal));
			AreaFrom = AreaTo;
			RemainingZones--;
		}
	}

	NatureZones.Sort();
	return NatureZones;
}

void LCGenerator::SubdivideFloorTiles(FBox2D Boundary, TArray<FNatureZone> Zones, TArray<FNatureZone>& FinalZones)
{
	// End of recursion, just one zone to assign
	if (Zones.Num() == 1)
	{
		Zones[0].Boundary = Boundary;
		FinalZones.Add(Zones[0]);
		return;
	}

	// Divide zones into two arrays of ~50% of area each
	int SumAreas = 0;
	TArray<FNatureZone> Zones1, Zones2;
	for(int i = 0; i < Zones.Num(); i++)
		if (SumAreas < Boundary.GetArea() / 2)
		{
			Zones1.Add(Zones[i]);
			SumAreas += Zones[i].Area;
		}
		else Zones2.Add(Zones[i]);
	
	// Calculate boundaries and subdivide
	float SmallEdge;
	FBox2D BigBoundary, SmallBoundary;
	if (Boundary.GetSize().X > Boundary.GetSize().Y)
	{
		// Split with vertical line
		SmallEdge = Boundary.GetSize().Y;
		BigBoundary = FBox2D(Boundary.Min, FVector2D(Boundary.Min.X + (SumAreas/SmallEdge), Boundary.Max.Y));
		SmallBoundary = FBox2D(FVector2D(Boundary.Min.X + (SumAreas / SmallEdge), Boundary.Min.Y), Boundary.Max);
	}
	else
	{
		// Split with horizontal line
		SmallEdge = Boundary.GetSize().X;
		BigBoundary = FBox2D(Boundary.Min, FVector2D(Boundary.Max.X, Boundary.Min.Y + (SumAreas / SmallEdge)));
		SmallBoundary = FBox2D(FVector2D(Boundary.Min.X, Boundary.Min.Y + (SumAreas / SmallEdge)), Boundary.Max);
	}
	SubdivideFloorTiles(BigBoundary, Zones1, FinalZones);
	SubdivideFloorTiles(SmallBoundary, Zones2, FinalZones);
}

void LCGenerator::PlaceQuadTreeIntoLevel(TLCQuadTree QuadTree, float Height)
{
	// Get all particles in QuadTree
	TArray<TLCParticle> FinalParticles;
	QuadTree.Query(QuadTree.Boundary, FinalParticles); // UE_LOG(LogTemp, Error, TEXT("There are %d final particles in the QuadTree"), FinalParticles.Num());

	// Place QuadTree items into World Level
	for (int i = 0; i < FinalParticles.Num(); i++)
	{
		FVector Position(FinalParticles[i].Center.X, FinalParticles[i].Center.Y, Height);
		FString Name = FinalParticles[i].Item->Asset->GetName() + FString::FromInt(i); // Name could be SelectedActorName+Random/City/Nature+[Row][Col]
		PlaceItemIntoLevel(FinalParticles[i].Item, Position, Name, QuadTree.Boundary);
	}
}

void LCGenerator::PlaceItemIntoLevel(ULCAsset* Item, FVector Position, FString Name, FBox2D Boundary)
{
	// Extract static mesh item
	UStaticMesh* MyStaticMesh = Cast<UStaticMesh>(Item->Asset); // PrintDebugUStaticMesh(MyStaticMesh);
	
	// Generate random transformation with the intervals
	float RandomScaleXYZ = FMath::FRandRange(Item->ScaleXYZ.Min, Item->ScaleXYZ.Max);
	float RandomPositionX = FMath::FRandRange(Item->TranslationX.Min, Item->TranslationX.Max);
	float RandomPositionY = FMath::FRandRange(Item->TranslationY.Min, Item->TranslationY.Max);
	float RandomPositionZ = FMath::FRandRange(Item->TranslationZ.Min, Item->TranslationZ.Max);
	float RandomRotationYaw = FMath::FRandRange(Item->RotationZ.Min, Item->RotationZ.Max);

	// Generate final actor transformation
	FRotator FinalRotation(0, RandomRotationYaw, 0);
	FVector FinalScale(RandomScaleXYZ, RandomScaleXYZ, RandomScaleXYZ);
	FVector FinalPosition = Position + FVector(RandomPositionX, RandomPositionY, RandomPositionZ - MyStaticMesh->GetBoundingBox().Min.Z);
	
	// Correct actor position to be inside boundaries
	float EdgeMinX = FinalPosition.X + (RandomScaleXYZ * MyStaticMesh->GetBoundingBox().Min.X);
	float EdgeMinY = FinalPosition.Y + (RandomScaleXYZ * MyStaticMesh->GetBoundingBox().Min.Y);
	float EdgeMaxX = FinalPosition.X + (RandomScaleXYZ * MyStaticMesh->GetBoundingBox().Max.X);
	float EdgeMaxY = FinalPosition.Y + (RandomScaleXYZ * MyStaticMesh->GetBoundingBox().Max.Y);
	
	if (EdgeMinX < Boundary.Min.X) FinalPosition.X += FMath::Abs(EdgeMinX - Boundary.Min.X);
	if (EdgeMinY < Boundary.Min.Y) FinalPosition.Y += FMath::Abs(EdgeMinY - Boundary.Min.Y);
	if (EdgeMaxX > Boundary.Max.X) FinalPosition.X -= FMath::Abs(EdgeMaxX - Boundary.Max.X);
	if (EdgeMaxY > Boundary.Max.Y) FinalPosition.Y -= FMath::Abs(EdgeMaxY - Boundary.Max.Y);

	FTransform ObjectTrasform(FinalRotation, FinalPosition, FinalScale);

	// Creating the actor and positioning it in the world
	UWorld* CurrentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* CurrentLevel = CurrentWorld->GetCurrentLevel();
	AStaticMeshActor* NewActorCreated = Cast<AStaticMeshActor>(GEditor->AddActor(CurrentLevel, AStaticMeshActor::StaticClass(), ObjectTrasform, true, RF_Public | RF_Standalone | RF_Transactional));

	NewActorCreated->GetStaticMeshComponent()->SetStaticMesh(MyStaticMesh);
	NewActorCreated->SetActorScale3D(ObjectTrasform.GetScale3D());
	NewActorCreated->SetActorLabel(Name);

	// Refresh editor
	GEditor->EditorUpdateComponents();
	NewActorCreated->GetStaticMeshComponent()->RegisterComponentWithWorld(CurrentWorld);
	CurrentWorld->UpdateWorldComponents(true, false);
	NewActorCreated->RerunConstructionScripts();
	GLevelEditorModeTools().MapChangeNotify();
}


/** DEBUG METHODS */
void LCGenerator::PrintDebugAActor(AActor* Actor)
{
	FString ActorName = *Actor->GetName();
	
	FVector Origin, Extent;
	Actor->GetActorBounds(false, Origin, Extent);

	UE_LOG(LogTemp, Error, TEXT("AActor: %s"), *ActorName);
	UE_LOG(LogTemp, Warning, TEXT("%s's Location is %s"), *ActorName, *Actor->GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s's Rotation is %s"), *ActorName, *Actor->GetActorRotation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s's Scale is %s"), *ActorName, *Actor->GetActorScale().ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s's Bounds/Origin is %s"), *ActorName, *Extent.ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s's Bounds/Extent is %s"), *ActorName, *Origin.ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s's BoundingBox is %s"), *ActorName, *Actor->GetComponentsBoundingBox().ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s's BoundingBox Extent is %s"), *ActorName, *Actor->GetComponentsBoundingBox().GetExtent().ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s's BoundingBox Size is %s"), *ActorName, *Actor->GetComponentsBoundingBox().GetSize().ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s's BoundingBox Center is %s"), *ActorName, *Actor->GetComponentsBoundingBox().GetCenter().ToString());

	//Actor->GetTransform().DebugPrint();
}

void LCGenerator::PrintDebugUStaticMesh(UStaticMesh* StaticMesh)
{
	FString StaticMeshName = *StaticMesh->GetName();

	UE_LOG(LogTemp, Error, TEXT("UStaticMesh: %s"), *StaticMeshName);
	UE_LOG(LogTemp, Warning, TEXT("%s's BoundingBox is %s"), *StaticMeshName, *StaticMesh->GetBoundingBox().ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s's BoundingBox Extent is %s"), *StaticMeshName, *StaticMesh->GetBoundingBox().GetExtent().ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s's BoundingBox Size is %s"), *StaticMeshName, *StaticMesh->GetBoundingBox().GetSize().ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s's BoundingBox Center is %s"), *StaticMeshName, *StaticMesh->GetBoundingBox().GetCenter().ToString());
}

void LCGenerator::PrintDebugFNatureZoneArray(TArray<FNatureZone> NatureZones)
{
	float Sum = 0;
	for (int i = 0; i < NatureZones.Num(); i++)
	{
		Sum += NatureZones[i].Area;
		FString Type;
		if (NatureZones[i].NatureType == ENatureType::Forest) Type = FString(TEXT("Forest"));
		if (NatureZones[i].NatureType == ENatureType::Desert) Type = FString(TEXT("Desert"));
		if (NatureZones[i].NatureType == ENatureType::Normal) Type = FString(TEXT("Normal"));

		UE_LOG(LogTemp, Warning, TEXT("ZONE %d: Type: %s, Area: %f, Boundary: %s"), i, *Type, NatureZones[i].Area, *NatureZones[i].Boundary.ToString());
	}
}

#undef LOCTEXT_NAMESPACE
