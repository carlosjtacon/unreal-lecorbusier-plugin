// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LeCorbusierEdMode.h"

const FEditorModeID FLeCorbusierEdMode::EM_LeCorbusierEdModeId = TEXT("EM_LeCorbusierEdMode");

FLeCorbusierEdMode::FLeCorbusierEdMode()
{

}

FLeCorbusierEdMode::~FLeCorbusierEdMode()
{

}

void FLeCorbusierEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FLeCorbusierEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FLeCorbusierEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FLeCorbusierEdMode::UsesToolkits() const
{
	return true;
}




