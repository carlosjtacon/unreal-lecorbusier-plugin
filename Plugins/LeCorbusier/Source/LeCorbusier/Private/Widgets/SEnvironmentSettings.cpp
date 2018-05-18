// Fill out your copyright notice in the Description page of Project Settings.

#include "SEnvironmentSettings.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "FSEnvironmentSettings"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEnvironmentSettings::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SHeader)
			.Content()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("EnvironmentSettings", "Environment Settings"))
			]
		]
		
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE