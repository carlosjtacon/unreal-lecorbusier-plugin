// Fill out your copyright notice in the Description page of Project Settings.

#include "SAssetLoader.h"
#include "SlateOptMacros.h"

//#include "EditorStyleSet.h"
//#include "ContentBrowserModule.h"
#include "AssetSelection.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"

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
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args(false, false, false, FDetailsViewArgs::HideNameArea, true);
	Args.bShowActorLabel = false;
	DetailsWidget = PropertyModule.CreateDetailView(Args);
	//DetailsWidget->SetVisibility(FoliageEditMode->UISettings.GetShowPaletteItemDetails() ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed);
	//DetailsWidget->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateSP(this, &SFoliagePalette::GetIsPropertyEditingEnabled));

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
		.AutoHeight()
		.Padding(5)
		[
			SNew(SBorder)
			[
				SNew(SOverlay)

				// Assets Tile View
				+ SOverlay::Slot()
				[
					SAssignNew(ListViewWidget, SListView<ULCAsset*>)
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

		+ SVerticalBox::Slot()
		.FillHeight(1)
		.Padding(5)
		[
			SNew(SBorder)
			[
				DetailsWidget.ToSharedRef()
			]
		]
	];
}

FReply SAssetLoader::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	//if (UICommandList->ProcessCommandBindings(InKeyEvent))
	if (InKeyEvent.GetKey() == EKeys::Delete)
	{
		UE_LOG(LogTemp, Warning, TEXT("Key Delete Pressed"));
		SAssetLoader::DeleteSelectedAssetFromList();
		return FReply::Handled();
	}

	return SCompoundWidget::OnKeyDown(MyGeometry, InKeyEvent);
}

TSharedRef<ITableRow> SAssetLoader::GenerateListRow(ULCAsset* Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return
		SNew(STableRow<UObject*>, OwnerTable)
		.Padding(2)
		[
			SNew(STextBlock)
			.Text(FText::FromName(Item->Asset->GetFName()))
		];
}

void SAssetLoader::ListSelectionChanged(ULCAsset* Item, ESelectInfo::Type SelectInfo)
{
	// TODO
	const bool bForceRefresh = true;
	// DetailsWidget->SetObjects(Item, bForceRefresh);
	DetailsWidget->SetObject(Item, bForceRefresh);
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
			ULCAsset* Obj = NewObject<ULCAsset>();
			Obj->Asset = AssetData.GetAsset();
			// UObject* Asset = AssetData.GetAsset();

			SAssetLoader::Items.Add(Obj);
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
	if (SAssetLoader::Items.Num() == 0) return EVisibility::Visible;

	if (FSlateApplication::Get().IsDragDropping())
	{
		TArray<FAssetData> DraggedAssets = AssetUtil::ExtractAssetDataFromDrag(FSlateApplication::Get().GetDragDroppingContent());
		for (const FAssetData& AssetData : DraggedAssets)
			if (AssetData.IsValid()) return EVisibility::Visible;
	}

	return EVisibility::Hidden;
}

void SAssetLoader::DeleteSelectedAssetFromList()
{
	Items.Remove(ListViewWidget->GetSelectedItems()[0]);
	ListViewWidget->RequestListRefresh();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE