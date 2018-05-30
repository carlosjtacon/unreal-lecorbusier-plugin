#include "LCGenerator.h"

void LCGenerator::CreateEnvironmentRandom(TArray<ULCAsset*> Items)
{
	UE_LOG(LogTemp, Warning, TEXT("BUILDING ENVIRONMENT RANDOM"));

	UObject* AssetObject = Items[0]->Asset;
	UStaticMesh* myStaticMesh = Cast<UStaticMesh>(AssetObject);
	
	PlacingStaticMesh(myStaticMesh);

	// create matrix ()
	// generar una matriz del tamano de los bounds de la superficie seleccionada
	// anadir en la matriz numeros que representan los items[0-n]
	
	// create environment ()
}

void LCGenerator::CreateEnvironmentNature(TArray<ULCAsset*> Items, bool bMixDifferentTrees)
{
	UE_LOG(LogTemp, Warning, TEXT("BUILDING ENVIRONMENT NATURE (bMixDifferentTrees: %s)"), (bMixDifferentTrees ? TEXT("True") : TEXT("False")));

}

void LCGenerator::CreateEnvironmentCity(TArray<ULCAsset*> Items, uint32 NumNatureAreas, float NaturePercentage)
{
	UE_LOG(LogTemp, Warning, TEXT("BUILDING ENVIRONMENT CITY (NumNatureAreas: %d, NaturePercentage: %f)"), NumNatureAreas, NaturePercentage);

}

void LCGenerator::PlacingStaticMesh(UStaticMesh* myStaticMesh)
{
	// Name & Transform
	FVector objectPosition(0, 0, 0);
	FRotator objectRotation(0, 0, 0); //in degrees
	FVector objectScale(1, 1, 1);
	FTransform objectTrasform(objectRotation, objectPosition, objectScale);

	// Creating the Actor and Positioning it in the World based in the Static Mesh
	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* currentLevel = currentWorld->GetCurrentLevel();
	UClass* staticMeshClass = AStaticMeshActor::StaticClass();

	AActor* newActorCreated = GEditor->AddActor(currentLevel, staticMeshClass, objectTrasform, true, RF_Public | RF_Standalone | RF_Transactional);
	AStaticMeshActor* smActor = Cast<AStaticMeshActor>(newActorCreated);

	smActor->GetStaticMeshComponent()->SetStaticMesh(myStaticMesh);
	smActor->SetActorScale3D(objectScale);
	// ID Name & Visible Name
	smActor->Rename(TEXT("MyStaticMeshInTheWorld"));
	smActor->SetActorLabel("MyStaticMeshInTheWorld");

	GEditor->EditorUpdateComponents();
	smActor->GetStaticMeshComponent()->RegisterComponentWithWorld(currentWorld);
	currentWorld->UpdateWorldComponents(true, false);
	smActor->RerunConstructionScripts();
	GLevelEditorModeTools().MapChangeNotify();
}