// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LeCorbusierEdModeToolkit.h"
#include "LeCorbusierEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"


#define LOCTEXT_NAMESPACE "FLeCorbusierEdModeToolkit"

FLeCorbusierEdModeToolkit::FLeCorbusierEdModeToolkit()
{
}

void FLeCorbusierEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	SAssignNew(ToolkitWidget, SBorder)
	[
		SNew(SVerticalBox)

		// SelectedActor is Plugin Land Input
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("HowToEnable", "Select the land to build the environment."))
		]
			
		// Environment Settings
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SAssignNew(EnvironmentSettingsWidget, SEnvironmentSettings)
		]

		// Assets and Details
		+ SVerticalBox::Slot()
		.FillHeight(1)
		.Padding(5)
		[
			SAssignNew(AssetLoaderWidget, SAssetLoader)
		]

		// Build Environment Button
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SNew(SButton)
			.Text(LOCTEXT("BuildButton", "Build Environment"))
			.IsEnabled(this, &FLeCorbusierEdModeToolkit::BuildButtonEnabled)
			.OnClicked(this, &FLeCorbusierEdModeToolkit::DoBuildEnvironment)
		]

	];
		
	FModeToolkit::Init(InitToolkitHost);
}

FName FLeCorbusierEdModeToolkit::GetToolkitFName() const
{
	return FName("LeCorbusierEdMode");
}

FText FLeCorbusierEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("LeCorbusierEdModeToolkit", "DisplayName", "LeCorbusierEdMode Tool");
}

class FEdMode* FLeCorbusierEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FLeCorbusierEdMode::EM_LeCorbusierEdModeId);
}

bool FLeCorbusierEdModeToolkit::BuildButtonEnabled() const
{
	return GEditor->GetSelectedActors()->Num() == 1
		&& AssetLoaderWidget->Items.Num() > 0;
}

FReply FLeCorbusierEdModeToolkit::DoBuildEnvironment()
{
	UE_LOG(LogTemp, Warning, TEXT("BUILDING ENVIRONMENT"));

	UObject* AssetObject = AssetLoaderWidget->Items[0]->Asset;
	UStaticMesh* myStaticMesh = Cast<UStaticMesh>(AssetObject);
	//LCGen.PlacingStaticMesh(StaticMesh);

	// Name & Transform
	FVector objectPosition(0, 0, 0);
	FRotator objectRotation(0, 0, 0); //in degrees
	FVector objectScale(1, 1, 1);
	FTransform objectTrasform(objectRotation, objectPosition, objectScale);

	// Creating the Actor and Positioning it in the World based in the Static Mesh
	// OLD // UWorld * currentWorld = GEditor->LevelViewportClients[0]->GetWorld();
	// OLD // UWorld * currentWorld = GEditor->GetSelectedActors()->GetWorld();
	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	// OLD // ULevel * currentLevel = currentWorld->GetLevel(0);
	ULevel * currentLevel = currentWorld->GetCurrentLevel();
	UClass * staticMeshClass = AStaticMeshActor::StaticClass();

	// @ https://answers.unrealengine.com/questions/659521/geditor-addactor-crash.html?sort=oldest
	//UWorld* const World = GEditor->GetEditorWorldContext().World();
	//AStaticMeshActor* newmesh = Cast<AStaticMeshActor>(GEditor->AddActor(World->GetCurrentLevel(), AStaticMeshActor::StaticClass(), FTransform(FVector(0))));

	AActor * newActorCreated = GEditor->AddActor(currentLevel, staticMeshClass, objectTrasform, true, RF_Public | RF_Standalone | RF_Transactional);
	
	AStaticMeshActor * smActor = Cast<AStaticMeshActor>(newActorCreated);

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

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
