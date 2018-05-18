// Fill out your copyright notice in the Description page of Project Settings.

#include "SAssetLoader.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "FSAssetLoader"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAssetLoader::Construct(const FArguments& InArgs)
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
				.Text(LOCTEXT("AssetContent", "Assets & Details"))
			]
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE