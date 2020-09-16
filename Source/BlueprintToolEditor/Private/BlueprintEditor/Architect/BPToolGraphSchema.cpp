#include "BPToolGraphSchema.h"
#include <BPToolGraphSchemaAction.h>
#include "Core/BoardNode.h"
#include "ToolMenus/Public/ToolMenuSection.h"
#include "ToolMenus/Public/ToolMenu.h"
#include "EdGraph/EdGraphNode.h"
#include "ToolMenus/Public/ToolMenuEntry.h"
#include <BPToolSchemaUtils.h>

#define LOCTEXT_NAMESPACE "BPToolGraphSchema"

UBPToolGraphSchema::UBPToolGraphSchema(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}


void UBPToolGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FBPToolGraphSchemaAction> NewNodeAction(new FBPToolGraphSchemaAction(
		LOCTEXT("A", "Grap Node"),
		LOCTEXT("Desc", "BoardNode"),
		LOCTEXT("NewGraphText", "Add a Node"),
		0));
	NewNodeAction->K3Node = NewObject<UBoardNode>(ContextMenuBuilder.OwnerOfTemporaries);
	ContextMenuBuilder.AddAction(NewNodeAction);
}

TSharedPtr<FEdGraphSchemaAction> UBPToolGraphSchema::GetCreateCommentAction() const
{
	return TSharedPtr<FEdGraphSchemaAction>(static_cast<FEdGraphSchemaAction*>(new FBPToolGraphSchemaAction));
}


void UBPToolGraphSchema::GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	
	FToolMenuSection& NewSection = Menu->AddSection("BPToolGraphSchema", LOCTEXT("", ""));
	FToolMenuEntry Args;

	NewSection.AddSubMenu(
		"MakeCollectionWith",
		NSLOCTEXT("ReferenceViewerSchema", "MakeCollectionWithTitle", "Make Collection with"),
		NSLOCTEXT("ReferenceViewerSchema", "MakeCollectionWithTooltip", "Makes a collection with either the referencers or dependencies of the selected nodes."),
		FNewToolMenuDelegate::CreateUObject(const_cast<UBPToolGraphSchema*>(this), &UBPToolGraphSchema::GetMakeCollectionWithSubMenu)
	);

	Super::GetContextMenuActions(Menu, Context);
}

void UBPToolGraphSchema::GetActionList(UEdGraph* OwnerBPGraph, TArray<TSharedPtr<FEdGraphSchemaAction> >& OutActions) const
{
	OutActions.Add(FBPToolSchemaUtils::CreateAction<FBPToolGraphSchemaAction, UBoardNode>(TEXT("Board"), TEXT("Board Tooltip"), OwnerBPGraph));
}


const FPinConnectionResponse UBPToolGraphSchema::CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const
{
	const UK3Node* OwningNodeA = Cast<UK3Node>(PinA->GetOwningNodeUnchecked());
	const UK3Node* OwningNodeB = Cast<UK3Node>(PinB->GetOwningNodeUnchecked());

	if (!OwningNodeA || !OwningNodeB)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Invalid nodes"));
	}
	if (OwningNodeA == OwningNodeB)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Both are on the same node"));
	}
	if (PinA->bOrphanedPin || PinB->bOrphanedPin)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot make new connections to orphaned pin"));
	}
	if (PinA->PinType.PinCategory == PinB->PinType.PinCategory)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT("Replace existing input connections"));
	}
	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("我不知道原因"));
}

void UBPToolGraphSchema::GetMakeCollectionWithSubMenu(UToolMenu* Menu)
{

}

#undef LOCTEXT_NAMESPACE