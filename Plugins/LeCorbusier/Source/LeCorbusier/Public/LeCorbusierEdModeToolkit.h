// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"

#include "Widgets/SEnvironmentSettings.h"
#include "Widgets/SAssetLoader.h"

#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"

class FLeCorbusierEdModeToolkit : public FModeToolkit
{
public:

	FLeCorbusierEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

private:

	TSharedPtr<SWidget> ToolkitWidget;
	TSharedPtr<SEnvironmentSettings> EnvironmentSettingsWidget;
	TSharedPtr<SAssetLoader> AssetLoaderWidget;

	bool BuildButtonEnabled() const;
	FReply DoBuildEnvironment();
};
