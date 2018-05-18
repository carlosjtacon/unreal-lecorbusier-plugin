// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LeCorbusier.h"
#include "LeCorbusierEdMode.h"

#define LOCTEXT_NAMESPACE "FLeCorbusierModule"

void FLeCorbusierModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FLeCorbusierEdMode>(
		FLeCorbusierEdMode::EM_LeCorbusierEdModeId, 
		LOCTEXT("LeCorbusierEdModeName", "LeCorbusierEdMode"), 
		FSlateIcon(), 
		true
		);
}

void FLeCorbusierModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FLeCorbusierEdMode::EM_LeCorbusierEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLeCorbusierModule, LeCorbusier)