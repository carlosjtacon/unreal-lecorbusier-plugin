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

	// Get item from list
	UObject* AssetObject = Items[0]->Asset;
	UStaticMesh* MyStaticMesh = Cast<UStaticMesh>(AssetObject);
	// PrintDebugUStaticMesh(MyStaticMesh);
	
	// Calculate needed variables
	float RealObjectZ = FloorSurface.Min.Z - MyStaticMesh->GetBoundingBox().Min.Z;
	
	// Generate actor location, rotation and scale
	FVector objectPosition(0, 0, RealObjectZ);
	FRotator objectRotation(0, 0, 0);
	FVector objectScale(1, 1, 1);
	FTransform objectTrasform(objectRotation, objectPosition, objectScale);
	
	// Place actor into the level
	PlacingStaticMesh(MyStaticMesh, objectTrasform);

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

void LCGenerator::PlacingStaticMesh(UStaticMesh* myStaticMesh, FTransform objectTrasform)
{
	// Creating the Actor and Positioning it in the World based in the Static Mesh
	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* currentLevel = currentWorld->GetCurrentLevel();
	UClass* staticMeshClass = AStaticMeshActor::StaticClass();

	AActor* newActorCreated = GEditor->AddActor(currentLevel, staticMeshClass, objectTrasform, true, RF_Public | RF_Standalone | RF_Transactional);
	AStaticMeshActor* smActor = Cast<AStaticMeshActor>(newActorCreated);

	smActor->GetStaticMeshComponent()->SetStaticMesh(myStaticMesh);
	// smActor->SetActorScale3D(objectScale);
	smActor->SetActorScale3D(objectTrasform.GetScale3D());
	// ID Name & Visible Name
	smActor->Rename(TEXT("MyStaticMeshInTheWorld"));
	smActor->SetActorLabel("MyStaticMeshInTheWorld");

	GEditor->EditorUpdateComponents();
	smActor->GetStaticMeshComponent()->RegisterComponentWithWorld(currentWorld);
	currentWorld->UpdateWorldComponents(true, false);
	smActor->RerunConstructionScripts();
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
