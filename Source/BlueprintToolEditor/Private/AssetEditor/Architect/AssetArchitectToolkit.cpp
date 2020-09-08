#include "AssetEditor/Architect/AssetArchitectToolkit.h"
#include "SBlueprintViewportClient.h"
#include "Widgets/Docking/SDockTab.h"
#include "EditorStyleSet.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "GraphEditor.h"
#include "Widgets/Text/STextBlock.h"
//#include "Architect/BPArchitectEdGraph.h"
//#include "Palette/BlueprintNodeListPalette.h"
#include "PropertyEditorModule.h"
#include "Toolkits/AssetEditorToolkit.h"

#define LOCTEXT_NAMESPACE "FBlueprintToolEditorToolkit"

namespace BlueprintToolEditorToolkit
{
	static const FName PreviewID("BPPreviewID");
	static const FName DetailsTabId("BPDetailsTabId");
	static const FName ContentBrowserID("BPContentBrowser");
}

FName FBlueprintToolEditorToolkit::GetToolkitFName() const
{
	return FName("BlueprintToolEditorToolkit");
}

FText FBlueprintToolEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("BlueprintToolLabel", "BlueprintTool Asset Editor");
}

FString FBlueprintToolEditorToolkit::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("BlueprintToolEditorPrefix","BlueprintData").ToString();
}

FLinearColor FBlueprintToolEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Red;
}

void FBlueprintToolEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_BlueprintToolEditor", "BlueprintTool Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(BlueprintToolEditorToolkit::PreviewID, 
		FOnSpawnTab::CreateSP(this, &FBlueprintToolEditorToolkit::HandleTabManagerSpawnTab,
			BlueprintToolEditorToolkit::PreviewID))
		.SetDisplayName(LOCTEXT("PreviewTabName", "Preview"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));

	InTabManager->RegisterTabSpawner(BlueprintToolEditorToolkit::DetailsTabId,
		FOnSpawnTab::CreateSP(this, &FBlueprintToolEditorToolkit::HandleTabManagerSpawnTab,
			BlueprintToolEditorToolkit::DetailsTabId))
		.SetDisplayName(LOCTEXT("DetailsTabName", "Details"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(BlueprintToolEditorToolkit::ContentBrowserID,
		FOnSpawnTab::CreateSP(this, &FBlueprintToolEditorToolkit::HandleTabManagerSpawnTab,
			BlueprintToolEditorToolkit::ContentBrowserID))
		.SetDisplayName(LOCTEXT("ContentBrowserTabName", "ContentBrowser"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.ContentBrowser"));
}

void FBlueprintToolEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(BlueprintToolEditorToolkit::PreviewID);
	InTabManager->UnregisterTabSpawner(BlueprintToolEditorToolkit::DetailsTabId);
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}

void FBlueprintToolEditorToolkit::Initialize(UBlueprintData* InTextAsset, const EToolkitMode::Type InMode, const TSharedPtr<IToolkitHost>& InToolkitHost)
{

	
	PreviewViewport = SNew(SBlueprintPreviewViewport)
		.BPEditorPtr(SharedThis(this))
		.ObjectToEdit(InTextAsset);
	// create tab layout
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("StandloneBlueprintLayout_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
				->SetHideTabWell(true)
				->SetSizeCoefficient(0.1f)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->SetSizeCoefficient(0.9f)
				->Split
				(
					FTabManager::NewSplitter()
					->SetOrientation(Orient_Vertical)
					//->SetSizeCoefficient(0.66f)
					->Split
					(
						FTabManager::NewSplitter()
						->SetOrientation(Orient_Horizontal)
						->SetSizeCoefficient(0.66f)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.85f)
							->SetHideTabWell(true)
							->AddTab(BlueprintToolEditorToolkit::PreviewID, ETabState::OpenedTab)
						)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.15f)
							->SetHideTabWell(true)
							->AddTab(BlueprintToolEditorToolkit::DetailsTabId, ETabState::OpenedTab)
						)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.33f)
						->SetHideTabWell(true)
						->AddTab(BlueprintToolEditorToolkit::ContentBrowserID, ETabState::OpenedTab)
					)
					
				)
			)
		);
	InitAssetEditor( 
		InMode,
		InToolkitHost,
		BlueprintToolEditorToolkit::PreviewID,
		Layout,
		true /*bCreateDefaultStandaloneMenu*/,
		true /*bCreateDefaultToolbar*/,
		InTextAsset
	);

	RegenerateMenusAndToolbars();
}

TSharedRef<SDockTab> FBlueprintToolEditorToolkit::HandleTabManagerSpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier)
{
	TSharedPtr<SWidget> TabWidget = SNullWidget::NullWidget;
	if (TabIdentifier == BlueprintToolEditorToolkit::PreviewID)
	{
		TabWidget = PreviewViewport.ToSharedRef();
	}
	else if (TabIdentifier == BlueprintToolEditorToolkit::DetailsTabId)
	{
	}
	else if (TabIdentifier == BlueprintToolEditorToolkit::ContentBrowserID)
	{
		TSharedRef<SDockTab> SpawnedTab =
			SNew(SDockTab)
			.Label(LOCTEXT("BPContentBrowserKey", "BluePrint Content Browser"))
			.TabColorScale(GetTabColorScale())
			[
				SNullWidget::NullWidget
			];
		IContentBrowserSingleton& ContentBrowserSingleton = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser").Get();
		FName ContentBrowerID = *("BP_ContentBrowser_" + FGuid::NewGuid().ToString());
		FContentBrowserConfig ContentBrowserConfig;
		TSharedRef<SWidget> ContentBrowser = ContentBrowserSingleton.CreateContentBrowser(ContentBrowerID, SpawnedTab, &ContentBrowserConfig);
		SpawnedTab->SetContent(ContentBrowser);

		return SpawnedTab;
	}
	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			TabWidget.ToSharedRef()
		];
}

#undef LOCTEXT_NAMESPACE