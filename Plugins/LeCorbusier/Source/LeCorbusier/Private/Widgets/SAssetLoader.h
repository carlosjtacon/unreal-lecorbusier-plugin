// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
//#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

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
	TArray<UObject*> Items;
	
	TSharedPtr<SListView<UObject*>> ListViewWidget;
	TSharedRef<ITableRow> GenerateListRow(UObject* Item, const TSharedRef<STableViewBase>& OwnerTable);
	void ListSelectionChanged(UObject* Item, ESelectInfo::Type SelectInfo);
	EVisibility GetListZoneVisibility() const;

	FReply HandleAssetDropped(const FGeometry& DropZoneGeometry, const FDragDropEvent& DragDropEvent);
	EVisibility GetDropZoneVisibility() const;
	
};
