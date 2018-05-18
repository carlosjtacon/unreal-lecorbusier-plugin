// Fill out your copyright notice in the Description page of Project Settings.

#include "SBuildButton.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "FSBuildButton"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SBuildButton::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SNew(SButton)
		.Text(LOCTEXT("BuildButton", "Build Environment"))
		// .OnClicked(this, &SBuildButton::DoBuild)
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE