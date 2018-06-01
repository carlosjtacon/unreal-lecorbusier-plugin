#include "LCGenerator.h"

#define LOCTEXT_NAMESPACE "LCGeneratorNamespace"


/** AVAILABLE ALGORITHMS */
void LCGenerator::CreateEnvironmentRandom(TArray<ULCAsset*> Items)
{
	UE_LOG(LogTemp, Warning, TEXT("BUILDING ENVIRONMENT RANDOM"));
	// Let editor know that we're about to do something that we want to undo/redo
	GEditor->BeginTransaction(LOCTEXT("CreateEnvironmentRandomTransaction", "Generate Environment"));
	
	// Get surface coordinates
	FBox FloorSurface = GetFloorSurface();
	UE_LOG(LogTemp, Warning, TEXT("FloorSurface is %s"), *FloorSurface.ToString());

	// Place actor into the level
	FVector Position(0, 0, FloorSurface.Min.Z);
	PlaceItemIntoLevel(Items[0], Position);

	// We're done generating the environment so we close the transaction
	GEditor->EndTransaction();
}

void LCGenerator::CreateEnvironmentNature(TArray<ULCAsset*> Items, bool bMixDifferentTrees)
{
	UE_LOG(LogTemp, Warning, TEXT("BUILDING ENVIRONMENT NATURE (bMixDifferentTrees: %s)"), (bMixDifferentTrees ? TEXT("True") : TEXT("False")));

}

void LCGenerator::CreateEnvironmentCity(TArray<ULCAsset*> Items, uint32 NumNatureAreas, float NaturePercentage)
{
	UE_LOG(LogTemp, Warning, TEXT("BUILDING ENVIRONMENT CITY (NumNatureAreas: %d, NaturePercentage: %f)"), NumNatureAreas, NaturePercentage);

}


/** UTILS METHODS */
FBox LCGenerator::GetFloorSurface()
{
	// Get selected actor as surface 
	TArray<UObject*> SelectedActors;
	GEditor->GetSelectedActors()->GetSelectedObjects(AActor::StaticClass(), SelectedActors);
	AActor* SelectedActor = Cast<AActor>(SelectedActors[0]);
	
	// PrintDebugAActor(SelectedActor);

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

void LCGenerator::PlaceItemIntoLevel(ULCAsset* Item, FVector Position)
{
	UStaticMesh* MyStaticMesh = Cast<UStaticMesh>(Item->Asset);
	// PrintDebugUStaticMesh(MyStaticMesh);

	// Calculate real z position
	float RealPositionX = Position.X;
	float RealPositionY = Position.Y;
	float RealPositionZ = Position.Z - MyStaticMesh->GetBoundingBox().Min.Z;

	// Generate actor location, rotation and scale
	float RotationYaw = MathAdvanced.FRandRange(Item->RotationZ.Min, Item->RotationZ.Max);
	FRotator RandomRotation(0, RotationYaw, 0);
	float PositionX = MathAdvanced.FRandRange(Item->TranslationX.Min, Item->TranslationX.Max);
	float PositionY = MathAdvanced.FRandRange(Item->TranslationY.Min, Item->TranslationY.Max);
	float PositionZ = MathAdvanced.FRandRange(Item->TranslationZ.Min, Item->TranslationZ.Max);
	FVector RandomPosition(PositionX + RealPositionX, PositionY + RealPositionY, PositionZ + RealPositionZ);
	float ScaleXYZ = MathAdvanced.FRandRange(Item->ScaleXYZ.Min, Item->ScaleXYZ.Max);
	FVector RandomScale(ScaleXYZ, ScaleXYZ, ScaleXYZ);
	FTransform ObjectTrasform(RandomRotation, RandomPosition, RandomScale);

	// Creating the Actor and Positioning it in the World based in the Static Mesh
	UWorld* CurrentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* CurrentLevel = CurrentWorld->GetCurrentLevel();
	AStaticMeshActor* NewActorCreated = Cast<AStaticMeshActor>(GEditor->AddActor(CurrentLevel, AStaticMeshActor::StaticClass(), ObjectTrasform, true, RF_Public | RF_Standalone | RF_Transactional));

	NewActorCreated->GetStaticMeshComponent()->SetStaticMesh(MyStaticMesh);
	NewActorCreated->SetActorScale3D(ObjectTrasform.GetScale3D());
	NewActorCreated->Rename(TEXT("MyStaticMeshInTheWorld"));
	NewActorCreated->SetActorLabel("MyStaticMeshInTheWorld");

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

#undef LOCTEXT_NAMESPACE
