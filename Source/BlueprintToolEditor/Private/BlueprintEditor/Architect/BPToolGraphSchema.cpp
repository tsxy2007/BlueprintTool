#include "BPToolGraphSchema.h"
#include <BPToolGraphSchemaAction.h>
#include "Core/BoardNode.h"

#define LOCTEXT_NAMESPACE "BPToolGraphSchema"

UBPToolGraphSchema::UBPToolGraphSchema(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}


void UBPToolGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FBPToolGraphSchemaAction> NewNodeAction(new FBPToolGraphSchemaAction(
		LOCTEXT("A", "Grap Node"),
		LOCTEXT("Desc", "MyTestBoard"),
		LOCTEXT("NewGraphText", "Add a Node"),
		0));
	NewNodeAction->K3Node = NewObject<UBoardNode>(ContextMenuBuilder.OwnerOfTemporaries);
	ContextMenuBuilder.AddAction(NewNodeAction);
}

TSharedPtr<FEdGraphSchemaAction> UBPToolGraphSchema::GetCreateCommentAction() const
{
	return TSharedPtr<FEdGraphSchemaAction>(static_cast<FEdGraphSchemaAction*>(new FBPToolGraphSchemaAction));
}

void UBPToolGraphSchema::GetActionList(UEdGraph* OwnerBPGraph, TArray<TSharedPtr<FEdGraphSchemaAction> >& OutActions) const
{
	TSharedPtr<FEdGraphSchemaAction> SchemaAction = MakeShareable(new FEdGraphSchemaAction);
	OutActions.Add(SchemaAction);
}

#undef LOCTEXT_NAMESPACE