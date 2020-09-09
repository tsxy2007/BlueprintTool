#include "SBPNodeLilstPalette.h"
#include "Modules/ModuleManager.h"
#include "Widgets/SOverlay.h"
#include "Styling/CoreStyle.h"
#include "EditorStyleSet.h"
//
//#include "EditorWidgetsModule.h"
#include "AssetRegistryModule.h"
#include "Widgets/Input/STextComboBox.h"
#include "BPToolGraphSchema.h"

#define LOCTEXT_NAMESPACE "BPNodeLilstPalettePalette"


//////////////////////////////////////////////////////////////////////////

void SBPNodeLilstPalettePalette::Construct(const FArguments& InArgs, TWeakPtr<FBlueprintToolEditorToolkit> InMaterialEditorPtr)
{
	this->BlueprintToolEditorToolkitPtr = InMaterialEditorPtr;
	this->ChildSlot
	[
		SNew(SBorder)
		.Padding(2.f)
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(GraphActionMenu, SGraphActionMenu)
					.OnActionDragged(this, &SBPNodeLilstPalettePalette::OnActionDragged)
					.OnCreateWidgetForAction(this, &SBPNodeLilstPalettePalette::OnCreateWidgetForAction)
					.OnCollectAllActions(this, &SBPNodeLilstPalettePalette::CollectAllActions)
				]
			]
		]
	];
}

TSharedRef<SWidget> SBPNodeLilstPalettePalette::OnCreateWidgetForAction(FCreateWidgetForActionData* const InCreateData)
{
	return	Super::OnCreateWidgetForAction(InCreateData);
}

void SBPNodeLilstPalettePalette::CollectAllActions(FGraphActionListBuilderBase& OutAllActions)
{
	const UBPToolGraphSchema* Schema = GetDefault<UBPToolGraphSchema>();

	FGraphActionMenuBuilder ActionMenuBuilder;

	TArray<TSharedPtr<FEdGraphSchemaAction>> Actions;
	
	if (BlueprintToolEditorToolkitPtr.IsValid())
	{
		TSharedPtr<SGraphEditor> BPGraphEditor = BlueprintToolEditorToolkitPtr.Pin()->GetGraphEditor();
		if (BPGraphEditor.IsValid())
		{
			UEdGraph* Graph = BPGraphEditor->GetCurrentGraph();
			Schema->GetActionList(Graph, Actions);

			for (TSharedPtr<FEdGraphSchemaAction> Action : Actions)
			{
				ActionMenuBuilder.AddAction(Action);
			}
		}
	}

	//@TODO: Avoid this copy
	OutAllActions.Append(ActionMenuBuilder);
}

FReply SBPNodeLilstPalettePalette::OnActionDragged(const TArray< TSharedPtr<FEdGraphSchemaAction> >& InActions, const FPointerEvent& MouseEvent)
{
	return Super::OnActionDragged(InActions, MouseEvent);
}

#undef LOCTEXT_NAMESPACE
