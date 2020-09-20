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
#include "Commands/BPToolCommands.h"
#include "Framework/Commands/GenericCommands.h"
#include "EdGraphUtilities.h"
#include "HAL/PlatformApplicationMisc.h"
#include "SNodePanel.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "BPToolGraphSchema.h"

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
		if (InTextAsset->BPToolGraph == nullptr)
		{
			InTextAsset->BPToolGraph = Cast<UBPToolArchitectEdGraph>(FBlueprintEditorUtils::CreateNewGraph(
				InTextAsset, 
				TEXT("BlueprintTool Tree"), 
				UBPToolArchitectEdGraph::StaticClass(), 
				UBPToolGraphSchema::StaticClass()));
		}
		UBPToolArchitectEdGraph* MyGraph = Cast<UBPToolArchitectEdGraph>(InTextAsset->BPToolGraph);
		MyGraph->InitializeGraph();
		GraphEditor = CreateBPGraphEditor(MyGraph);
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
	DetailsView->SetObjects(SelectionNode.Array());
}


void FBlueprintToolEditorToolkit::CreateCommands()
{
	FBPToolCommands::Register();

	if (!GraphEditorCommands.IsValid())
	{
		GraphEditorCommands = MakeShareable(new FUICommandList);
		/*GraphEditorCommands->MapAction(FGenericCommands::Get().Rename,
			FExecuteAction::CreateSP(this, &FBlueprintEditor::OnRenameNode),
			FCanExecuteAction::CreateSP(this, &FBlueprintEditor::CanRenameNodes)
		);*/

		GraphEditorCommands->MapAction(FGenericCommands::Get().Undo,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::UndoGraphAction));
		GraphEditorCommands->MapAction(FGenericCommands::Get().Redo,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::RedoGraphAction));
		GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::SelectAllNodes),
			FCanExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CanSelectAllNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CopySelectedNodes),
			FCanExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CanCopyNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::PasteNodes),
			FCanExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CanPasteNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CutSelectedNodes),
			FCanExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CanCutNodes)
		);
		GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::DuplicateNodes),
			FCanExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CanDuplicateNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::DeleteSelectedNodes),
			FCanExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CanDeleteNode)
		);
	}

}


void FBlueprintToolEditorToolkit::DeleteSelectedNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GraphEditor;
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());
	CurrentGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*NodeIt))
		{
			if (Node->CanUserDeleteNode())
			{
				Node->Modify();
				Node->DestroyNode();
			}
		}
	}
}


bool FBlueprintToolEditorToolkit::CanDeleteNode() const
{
	return true;
}


void FBlueprintToolEditorToolkit::CopySelectedNodes()
{
	const FGraphPanelSelectionSet SelectNodes = GraphEditor->GetSelectedNodes();
	FString ExportedText;
	FEdGraphUtilities::ExportNodesToText(SelectNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}


bool FBlueprintToolEditorToolkit::CanCopyNodes() const
{
	return true;
}


void FBlueprintToolEditorToolkit::PasteNodes()
{
	const FScopedTransaction Transaction(NSLOCTEXT("BPToolNodes", "BPToolNodesPaste", "Hello Node We are paste!!"));

	GraphEditor->ClearSelectionSet();

	GraphEditor->GetCurrentGraph()->Modify();

	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);

	TSet<UEdGraphNode*> PasteNodes;
	FEdGraphUtilities::ImportNodesFromText(GraphEditor->GetCurrentGraph(), TextToImport, PasteNodes);

	FVector2D AvgNodePosition(0.f, 0.f);
	for (TSet<UEdGraphNode*>::TIterator It(PasteNodes);It;++It)
	{
		UEdGraphNode* Node = *It;
		AvgNodePosition.X += Node->NodePosX;
		AvgNodePosition.Y += Node->NodePosY;
	}

	if (PasteNodes.Num() > 0)
	{
		float InvNumNodes = 1.f / float(PasteNodes.Num());
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;
	}
	const FVector2D Location = GraphEditor->GetPasteLocation();
	for (TSet<UEdGraphNode*>::TIterator It(PasteNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;
		GraphEditor->SetNodeSelection(Node, true);

		Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + Location.X;
		Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + Location.Y;

		Node->SnapToGrid(SNodePanel::GetSnapGridSize());

		Node->CreateNewGuid();


		for (UEdGraphPin* Pin : Node->Pins)
		{
			Pin->PinId = FGuid::NewGuid();
		}
	}
	GraphEditor->NotifyGraphChanged();
}


bool FBlueprintToolEditorToolkit::CanPasteNodes() const
{
	return true;
}


void FBlueprintToolEditorToolkit::CutSelectedNodes()
{
	CopySelectedNodes();

	const FGraphPanelSelectionSet OldSelectedNodes = GraphEditor->GetSelectedNodes();

	FGraphPanelSelectionSet RemainingNodes;
	GraphEditor->ClearSelectionSet();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if ((Node != nullptr)&& Node->CanDuplicateNode())
		{
			GraphEditor->SetNodeSelection(Node, true);
		}
		else
		{
			RemainingNodes.Add(Node);
		}
	}

	DeleteSelectedNodes();

	GraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectIter(RemainingNodes);SelectIter;++SelectIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectIter))
		{
			GraphEditor->SetNodeSelection(Node, true);
		}
	}
}


bool FBlueprintToolEditorToolkit::CanCutNodes() const
{
	return true;
}


void FBlueprintToolEditorToolkit::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}


bool FBlueprintToolEditorToolkit::CanDuplicateNodes() const
{
	return CanCopyNodes()&&CanPasteNodes();
}


void FBlueprintToolEditorToolkit::SelectAllNodes()
{
	if (GraphEditor.IsValid())
	{
		GraphEditor->SelectAllNodes();
	}
}


bool FBlueprintToolEditorToolkit::CanSelectAllNodes() const
{
	return GraphEditor.IsValid();
}


void FBlueprintToolEditorToolkit::UndoGraphAction()
{
	GEditor->UndoTransaction();
	GraphEditor->NotifyGraphChanged();
}


void FBlueprintToolEditorToolkit::RedoGraphAction()
{
	GEditor->RedoTransaction();
	GraphEditor->NotifyGraphChanged();
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
		

	//创建命令
	CreateCommands();

	TSharedRef<SGraphEditor> GraphEditorInstance = SNew(SGraphEditor)
		.GraphToEdit(InGraph)
		.Appearance(AppearanceInfo)
		.TitleBar(TitleBar)
		.GraphEvents(InGraphEvents)
		.AdditionalCommands(GraphEditorCommands);
	return GraphEditorInstance;
}

#undef LOCTEXT_NAMESPACE