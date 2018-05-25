// Fill out your copyright notice in the Description page of Project Settings.

#include "SAssetLoader.h"
#include "SlateOptMacros.h"

//#include "EditorStyleSet.h"
//#include "ContentBrowserModule.h"
#include "AssetSelection.h"

#define LOCTEXT_NAMESPACE "FSAssetLoader"

// Asset Drag and Drop Zone
class SAssetDragDropZone : public SCompoundWidget
{
	public:
	SLATE_BEGIN_ARGS(SAssetDragDropZone) {}
		SLATE_EVENT(FOnDrop, OnDrop)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		bIsDragOn = false;
		OnDropDelegate = InArgs._OnDrop;

		ChildSlot
		[
			SNew(SBorder)
			.BorderBackgroundColor(this, &SAssetDragDropZone::GetBackgroundColor)
			.Padding(FMargin(30))
			[
				SNew(STextBlock)
				.Text(LOCTEXT("DropAssets", "Drop Assets Here"))
			]
		];
	}

	FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override
	{
		bIsDragOn = false;
		if (OnDropDelegate.IsBound())
		{
			return OnDropDelegate.Execute(MyGeometry, DragDropEvent);
		}
		
		return FReply::Handled();
	}

	virtual void OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override
	{
		bIsDragOn = true;
	}

	virtual void OnDragLeave(const FDragDropEvent& DragDropEvent) override
	{
		bIsDragOn = false;
	}

private:
	FSlateColor GetBackgroundColor() const
	{
		return bIsDragOn ? FLinearColor(0.0f, 1.0f, 0.0f, 1.0f) : FLinearColor(1.0f, 0.6f, 0.1f, 1.0f);
	}

private:
	FOnDrop OnDropDelegate;
	bool bIsDragOn;
};

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

		+ SVerticalBox::Slot()
		[
			SNew(SBorder)
			[
				SNew(SOverlay)

				// Assets Tile View
				+ SOverlay::Slot()
				[
					SAssignNew(ListViewWidget, SListView<UObject*>)
					.ItemHeight(24)
					.ClearSelectionOnClick(false)
					.SelectionMode(ESelectionMode::Single)
					.ListItemsSource(&Items)
					.Visibility(this, &SAssetLoader::GetListZoneVisibility)
					.OnGenerateRow(this, &SAssetLoader::GenerateListRow)
					.OnSelectionChanged(this, &SAssetLoader::ListSelectionChanged)
				]

				// Foliage Mesh Drop Zone
				+ SOverlay::Slot()
				.Padding(3)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SAssetDragDropZone)
					.Visibility(this, &SAssetLoader::GetDropZoneVisibility)
					.OnDrop(this, &SAssetLoader::HandleAssetDropped)
				]
			]
		]
	];
}

TSharedRef<ITableRow> SAssetLoader::GenerateListRow(UObject* Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return
		SNew(STableRow<UObject*>, OwnerTable)
		.Padding(2)
		[
			SNew(STextBlock)
			.Text(FText::FromName(Item->GetFName()))
		];
}

void SAssetLoader::ListSelectionChanged(UObject* Item, ESelectInfo::Type SelectInfo)
{
	// TODO
}

EVisibility SAssetLoader::GetListZoneVisibility() const
{
	if (FSlateApplication::Get().IsDragDropping())
	{
		TArray<FAssetData> DraggedAssets = AssetUtil::ExtractAssetDataFromDrag(FSlateApplication::Get().GetDragDroppingContent());
		for (const FAssetData& AssetData : DraggedAssets)
			if (AssetData.IsValid()) return EVisibility::Hidden;
	}

	return EVisibility::Visible;
}

FReply SAssetLoader::HandleAssetDropped(const FGeometry& DropZoneGeometry, const FDragDropEvent& DragDropEvent)
{
	TArray<FAssetData> DroppedAssetData = AssetUtil::ExtractAssetDataFromDrag(DragDropEvent);
	if (DroppedAssetData.Num() > 0)
	{
		// Treat the entire drop as a transaction (in case multiples types are being added)
		// const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "FoliageMode_DragDropTypesTransaction", "Drag-drop Foliage"));

		for (auto& AssetData : DroppedAssetData)
		{
			// if (AddFoliageTypeCombo.IsValid())
			// {
			// 	AddFoliageTypeCombo->SetIsOpen(false);
			// }

			// GWarn->BeginSlowTask(LOCTEXT("AddFoliageType_LoadPackage", "Loading Foliage Type"), true, false);
			UObject* Asset = AssetData.GetAsset();

			SAssetLoader::Items.Add(Asset);
			UE_LOG(LogTemp, Warning, TEXT("Dropped AssetData"));
			ListViewWidget->RequestListRefresh();
			// AssetData.PrintAssetData();

			// GWarn->EndSlowTask();

			// FoliageEditMode->AddFoliageAsset(Asset);
		}
	}

	return FReply::Handled();
}

EVisibility SAssetLoader::GetDropZoneVisibility() const
{
	if (FSlateApplication::Get().IsDragDropping())
	{
		TArray<FAssetData> DraggedAssets = AssetUtil::ExtractAssetDataFromDrag(FSlateApplication::Get().GetDragDroppingContent());
		for (const FAssetData& AssetData : DraggedAssets)
			if (AssetData.IsValid()) return EVisibility::Visible;
	}

	return EVisibility::Hidden;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE