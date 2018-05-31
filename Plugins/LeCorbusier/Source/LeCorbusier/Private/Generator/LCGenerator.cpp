#include "LCGenerator.h"

#define LOCTEXT_NAMESPACE "LCGeneratorNamespace"

void LCGenerator::CreateEnvironmentRandom(TArray<ULCAsset*> Items)
{
	UE_LOG(LogTemp, Warning, TEXT("BUILDING ENVIRONMENT RANDOM"));
	// Let editor know that we're about to do something that we want to undo/redo
	GEditor->BeginTransaction(LOCTEXT("CreateEnvironmentRandomTransaction", "Generate Environment"));

	// Get selected actor as surface 
	TArray<UObject*> SelectedActors;
	GEditor->GetSelectedActors()->GetSelectedObjects(AActor::StaticClass(), SelectedActors);
	AActor* SelectedActor = Cast<AActor>(SelectedActors[0]);
	UE_LOG(LogTemp, Warning, TEXT("Building environment using %s as surface"), *SelectedActor->GetName());
	
	// Bounds Origin = center of box
	// Bounds Extent = distance of bounds from origin
	FVector Origin, Extent;
	SelectedActor->GetActorBounds(false, Origin, Extent);
	//SelectedActor->GetTransform().DebugPrint();
	UE_LOG(LogTemp, Warning, TEXT("SelectedActor's Location is %s"), *SelectedActor->GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("SelectedActor's Rotation is %s"), *SelectedActor->GetActorRotation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("SelectedActor's Scale is %s"), *SelectedActor->GetActorScale().ToString());
	UE_LOG(LogTemp, Warning, TEXT("SelectedActor's Bounds Origin is %s"), *Extent.ToString());
	UE_LOG(LogTemp, Warning, TEXT("SelectedActor's Bounds Extent is %s"), *Origin.ToString());

	// Get item from list
	UObject* AssetObject = Items[0]->Asset;
	UStaticMesh* myStaticMesh = Cast<UStaticMesh>(AssetObject);
	UE_LOG(LogTemp, Warning, TEXT("myStaticMesh's BoundingBox is %s"), *myStaticMesh->GetBoundingBox().GetSize().ToString());

	// Generate actor location, rotation and scale
	FVector objectPosition(0, 0, 0);
	FRotator objectRotation(0, 0, 0);
	FVector objectScale(1, 1, 1);
	FTransform objectTrasform(objectRotation, objectPosition, objectScale);
	
	// Place actor into the level
	PlacingStaticMesh(myStaticMesh, objectTrasform);

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

#undef LOCTEXT_NAMESPACE
