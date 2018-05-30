#include "LCGenerator.h"

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