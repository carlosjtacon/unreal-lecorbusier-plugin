// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
//#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

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

private:
	TArray<ULCAsset*> Items;
	TSharedPtr<SListView<ULCAsset*>> ListViewWidget;
	TSharedRef<ITableRow> GenerateListRow(ULCAsset* Item, const TSharedRef<STableViewBase>& OwnerTable);
	void ListSelectionChanged(ULCAsset* Item, ESelectInfo::Type SelectInfo);
	EVisibility GetListZoneVisibility() const;

	FReply HandleAssetDropped(const FGeometry& DropZoneGeometry, const FDragDropEvent& DragDropEvent);
	EVisibility GetDropZoneVisibility() const;

	TSharedPtr<class IDetailsView> DetailsWidget;
	
};
