// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateOptMacros.h"
#include "AssetSelection.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "Engine/StaticMesh.h"


#include "../Types/ULCAsset.h"

/**
 * 
 */
class SAssetLoader : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAssetLoader)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	TArray<ULCAsset*> Items;

private:
	TSharedPtr<SListView<ULCAsset*>> ListViewWidget;
	TSharedPtr<class IDetailsView> DetailsWidget;

	TSharedRef<ITableRow> GenerateListRow(ULCAsset* Item, const TSharedRef<STableViewBase>& OwnerTable);
	void ListSelectionChanged(ULCAsset* Item, ESelectInfo::Type SelectInfo);
	EVisibility GetListZoneVisibility() const;

	FReply HandleAssetDropped(const FGeometry& DropZoneGeometry, const FDragDropEvent& DragDropEvent);
	EVisibility GetDropZoneVisibility() const;

	void DeleteSelectedAssetFromList();
	
};
