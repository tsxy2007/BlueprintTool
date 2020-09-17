#include "AssetEditor/Architect/AssetArchitectToolkit.h"
#include "SBlueprintViewportClient.h"
#include "Widgets/Docking/SDockTab.h"
#include "EditorStyleSet.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "GraphEditor.h"
#include "Widgets/Text/STextBlock.h"
#include "PropertyEditorModule.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "BPToolArchitectEdGraph.h"
#include "EdGraph/EdGraph.h"
#include <SBPNodeLilstPalette.h>

#define LOCTEXT_NAMESPACE "FBlueprintToolEditorToolkit"

namespace BlueprintToolEditorToolkit
{
	static const FName BPViewportID("BPViewportID");
	static const FName DetailsTabId("BPDetailsTabId");
	static const FName ContentBrowserID("BPContentBrowser");
	static const FName GraphID("BPGraph");
	static const FName BPNodeListID("BPNodeList");
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

	InTabManager->RegisterTabSpawner(BlueprintToolEditorToolkit::BPViewportID, 
		FOnSpawnTab::CreateSP(this, &FBlueprintToolEditorToolkit::HandleTabManagerSpawnTab,
			BlueprintToolEditorToolkit::BPViewportID))
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

	InTabManager->RegisterTabSpawner(BlueprintToolEditorToolkit::GraphID,
		FOnSpawnTab::CreateSP(this, &FBlueprintToolEditorToolkit::HandleTabManagerSpawnTab,
			BlueprintToolEditorToolkit::GraphID))
		.SetDisplayName(LOCTEXT("GraphTabName", "Graph"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(BlueprintToolEditorToolkit::BPNodeListID,
		FOnSpawnTab::CreateSP(this, &FBlueprintToolEditorToolkit::HandleTabManagerSpawnTab,
			BlueprintToolEditorToolkit::BPNodeListID))
		.SetDisplayName(LOCTEXT("NodeListTabName", "NodeList"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FBlueprintToolEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(BlueprintToolEditorToolkit::BPViewportID);
	InTabManager->UnregisterTabSpawner(BlueprintToolEditorToolkit::DetailsTabId);
	InTabManager->UnregisterTabSpawner(BlueprintToolEditorToolkit::BPNodeListID);
	InTabManager->UnregisterTabSpawner(BlueprintToolEditorToolkit::ContentBrowserID);
	InTabManager->UnregisterTabSpawner(BlueprintToolEditorToolkit::GraphID);
	
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}

void FBlueprintToolEditorToolkit::Initialize(UBlueprintData* InTextAsset, const EToolkitMode::Type InMode, const TSharedPtr<IToolkitHost>& InToolkitHost)
{
	//创建蓝图
	{
		UBPToolArchitectEdGraph* Graph = NewObject<UBPToolArchitectEdGraph>(InTextAsset,UBPToolArchitectEdGraph::StaticClass(),NAME_None,RF_Transactional);
		Graph->InitializeGraph();
		GraphEditor = CreateBPGraphEditor(Graph);
	}
	PreviewViewport = SNew(SBlueprintPreviewViewport)
		.BPEditorPtr(SharedThis(this))
		.ObjectToEdit(InTextAsset);

	
	// create tab layout
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("StandaloneBPTool_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->SetHideTabWell(true)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(0.9f)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)->SetSizeCoefficient(0.2f)
					->Split
					(
						FTabManager::NewStack()
						->SetHideTabWell(true)
						->AddTab(BlueprintToolEditorToolkit::BPViewportID, ETabState::OpenedTab)
					)
					->Split
					(
						FTabManager::NewStack()
						->AddTab(BlueprintToolEditorToolkit::DetailsTabId, ETabState::OpenedTab)
					)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->SetSizeCoefficient(0.80f)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.8f)
						->SetHideTabWell(true)
						->AddTab(BlueprintToolEditorToolkit::GraphID, ETabState::OpenedTab)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.20f)
						->AddTab(BlueprintToolEditorToolkit::ContentBrowserID, ETabState::OpenedTab)
					)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(0.2f)
					->Split
					(
						FTabManager::NewStack()
						->AddTab(BlueprintToolEditorToolkit::BPNodeListID, ETabState::OpenedTab)
					)
				)
			)
		);
	InitAssetEditor( 
		InMode,
		InToolkitHost,
		BlueprintToolEditorToolkit::BPViewportID,
		StandaloneDefaultLayout,
		true /*bCreateDefaultStandaloneMenu*/,
		true /*bCreateDefaultToolbar*/,
		InTextAsset
	);

	RegenerateMenusAndToolbars();
}


void FBlueprintToolEditorToolkit::OnSelectedBPNodesChanged(const TSet<class UObject*>& SelectionNode)
{
	if (SelectionNode.Num() > 0)
	{
		DetailsView->SetObjects(SelectionNode.Array());
	}
}

TSharedRef<SDockTab> FBlueprintToolEditorToolkit::HandleTabManagerSpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier)
{
	TSharedPtr<SWidget> TabWidget = SNullWidget::NullWidget;
	if (TabIdentifier == BlueprintToolEditorToolkit::BPViewportID)
	{
		TabWidget = PreviewViewport.ToSharedRef();
	}
	else if (TabIdentifier == BlueprintToolEditorToolkit::DetailsTabId)
	{
		FPropertyEditorModule& EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

		FDetailsViewArgs DetailsViewArgs(
			/*bUpdateFromSelection=*/ false,
			/*bLockable=*/ false,
			/*bAllowSearch=*/ false,
			FDetailsViewArgs::HideNameArea,
			/*bHideSelectionTip=*/ true,
			/*InNotifyHook=*/ nullptr,
			/*InSearchInitialKeyFocus=*/ false,
			/*InViewIdentifier=*/ NAME_None);
		DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Automatic;
		DetailsViewArgs.bShowOptions = false;
		DetailsViewArgs.bAllowMultipleTopLevelObjects = true;

		 TabWidget = DetailsView = EditModule.CreateDetailView(DetailsViewArgs);

	}
	else if (TabIdentifier == BlueprintToolEditorToolkit::GraphID)
	{
		TabWidget = SNew(SOverlay)
			+ SOverlay::Slot()
			[
				GraphEditor.ToSharedRef()
			];
	}
	else if (TabIdentifier == BlueprintToolEditorToolkit::BPNodeListID)
	{
		TabWidget = SNew(SBPNodeLilstPalettePalette, SharedThis(this));
	}
	else if (TabIdentifier == BlueprintToolEditorToolkit::ContentBrowserID)
	{
		IContentBrowserSingleton& ContentBrowserSingleton = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser").Get();
		FName ContentBrowerID = *("BP_ContentBrowser_" + FGuid::NewGuid().ToString());
		FContentBrowserConfig ContentBrowserConfig;
		TabWidget = ContentBrowserSingleton.CreateContentBrowser("VRContentBrowser", nullptr, &ContentBrowserConfig);
	}
	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			TabWidget.ToSharedRef()
		];
}

TSharedRef<SGraphEditor> FBlueprintToolEditorToolkit::CreateBPGraphEditor(UEdGraph* InGraph)
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("MyGraphAppearanceInfo", "BPTool");

	TSharedRef<SWidget> TitleBar = SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("MyBPTitleBar","BPToolEditor"))
				.TextStyle(FEditorStyle::Get(),TEXT("GraphBreadcrumbButtonText"))
			]
		];



	SGraphEditor::FGraphEditorEvents InGraphEvents;
	InGraphEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FBlueprintToolEditorToolkit::OnSelectedBPNodesChanged);
		
	TSharedRef<SGraphEditor> GraphEditorInstance = SNew(SGraphEditor)
		.GraphToEdit(InGraph)
		.Appearance(AppearanceInfo)
		.TitleBar(TitleBar)
		.GraphEvents(InGraphEvents);
	return GraphEditorInstance;
}

#undef LOCTEXT_NAMESPACE