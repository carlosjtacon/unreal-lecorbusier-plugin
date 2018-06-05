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

	// Matrix generation
	// Unreal units = cm / Matrix units = m
	int CONVERSION_FACTOR = 100;
	TArray<TArray<ULCAsset*>> Matrix;
	FVector MatrixSize = FloorSurface.GetSize() / CONVERSION_FACTOR;
	UE_LOG(LogTemp, Warning, TEXT("Matrix Size %s"), *MatrixSize.ToString());
	Matrix.SetNum(MatrixSize.X); for (int i = 0; i < Matrix.Num(); i++) Matrix[i].SetNum(MatrixSize.Y);
	
	ULCAsset* EmptyItem = NewObject<ULCAsset>();
	for(int i = 0; i < Items.Num(); i++)
	{
		uint32 ItemCount = 0;
		ULCAsset* Item = Items[i];
		int32 GridRadius = MathBasics.CeilToInt(Item->Radius / CONVERSION_FACTOR);
		for (int x = 0; x < Matrix.Num(); x+= GridRadius)
			for (int y = 0; y < Matrix[x].Num(); y += GridRadius)
			{
				bool bInstancesAvailable = (Item->MaxInstances == 0 || Item->MaxInstances > ItemCount);
				if (Item->bEnable && bInstancesAvailable && Matrix[x][y] == nullptr)
				{
					if (Item->Probability > MathBasics.FRand())
					{
						Matrix[x][y] = Item;
						ItemCount++;
					}
				}
			}
	}

	// Place items stored in matrix into the world
	for (int i = 0; i < Matrix.Num(); i++)
	{
		for (int j = 0; j < Matrix[i].Num(); j++)
			if (Matrix[i][j] != EmptyItem && IsValid(Matrix[i][j]))
			{
				FVector Position(FloorSurface.Max.X - i * CONVERSION_FACTOR, FloorSurface.Min.Y + j * CONVERSION_FACTOR, FloorSurface.Min.Z);
				FString Name = Matrix[i][j]->GetName() + FString::FromInt(i) + FString::FromInt(j); // Name could be SelectedActorName+Random/City/Nature+[Row][Col]
				PlaceItemIntoLevel(Matrix[i][j], Position, Name);
			}
	}


	// We're done generating the environment so we close the transaction
	GEditor->EndTransaction();
}

void LCGenerator::CreateEnvironmentNature(TArray<ULCAsset*> Items, bool bMixDifferentTrees)
{
	UE_LOG(LogTemp, Warning, TEXT("BUILDING ENVIRONMENT NATURE (bMixDifferentTrees: %s)"), (bMixDifferentTrees ? TEXT("True") : TEXT("False")));
	// Let editor know that we're about to do something that we want to undo/redo
	GEditor->BeginTransaction(LOCTEXT("CreateEnvironmentRandomTransaction", "Generate Environment"));

	// Get surface coordinates
	FBox FloorSurface = GetFloorSurface();
	FBox2D FloorSurface2D(FVector2D(FloorSurface.Min.X, FloorSurface.Min.Y), FVector2D(FloorSurface.Max.X, FloorSurface.Max.Y));
	UE_LOG(LogTemp, Warning, TEXT("FloorSurface is %s"), *FloorSurface.ToString());

	
	//todo: cogemos un punto random en el qtree, si en un radio X encontramos otro arbol 
	//hay una probabilidad muy grande de que caiga, si no hay una probabilidad muy peque�a
	//radius necesario?

	TLCQuadTree QuadTree(FloorSurface2D, 4);
	for (int i = 0; i < 20; i++)
	{
		float X = MathAdvanced.FRandRange(FloorSurface2D.Min.X, FloorSurface2D.Max.X);
		float Y = MathAdvanced.FRandRange(FloorSurface2D.Min.Y, FloorSurface2D.Max.Y);
		TLCParticle Particle(FVector2D(X, Y), 10);
		QuadTree.Insert(Particle);
		UE_LOG(LogTemp, Warning, TEXT("Inserted Particle: %s"), *Particle.ToString());
	}
	
	TArray<TLCParticle> Particles;
	QuadTree.Query(FloorSurface2D, Particles);
	UE_LOG(LogTemp, Warning, TEXT("(Box) There are %d particles in the QuadTree"), Particles.Num());

	Particles.Empty();
	TLCParticle Circle(FloorSurface2D.GetCenter(), FloorSurface2D.GetExtent().X);
	QuadTree.Query(Circle, Particles);
	UE_LOG(LogTemp, Warning, TEXT("(Circle) There are %d particles in the QuadTree"), Particles.Num());




	// We're done generating the environment so we close the transaction
	GEditor->EndTransaction();
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

void LCGenerator::PlaceItemIntoLevel(ULCAsset* Item, FVector Position, FString Name)
{
	// Extract static mesh item
	UStaticMesh* MyStaticMesh = Cast<UStaticMesh>(Item->Asset); // PrintDebugUStaticMesh(MyStaticMesh);
	
	// Generate random transformation with the intervals
	float RandomScaleXYZ = MathAdvanced.FRandRange(Item->ScaleXYZ.Min, Item->ScaleXYZ.Max);
	float RandomPositionX = MathAdvanced.FRandRange(Item->TranslationX.Min, Item->TranslationX.Max);
	float RandomPositionY = MathAdvanced.FRandRange(Item->TranslationY.Min, Item->TranslationY.Max);
	float RandomPositionZ = MathAdvanced.FRandRange(Item->TranslationZ.Min, Item->TranslationZ.Max);
	float RandomRotationYaw = MathAdvanced.FRandRange(Item->RotationZ.Min, Item->RotationZ.Max);

	// Generate final actor transformation
	FVector RandomPosition(RandomPositionX, RandomPositionY, RandomPositionZ);
	FVector OffsetPosition(MyStaticMesh->GetBoundingBox().Max.X, MyStaticMesh->GetBoundingBox().Min.Y, MyStaticMesh->GetBoundingBox().Min.Z);
	FVector FinalPosition = Position + RandomPosition - RandomScaleXYZ * OffsetPosition;
	FVector FinalScale(RandomScaleXYZ, RandomScaleXYZ, RandomScaleXYZ);
	FRotator FinalRotation(0, RandomRotationYaw, 0);
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

#undef LOCTEXT_NAMESPACE
