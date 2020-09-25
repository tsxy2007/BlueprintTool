#include "BPToolGraphSchema.h"
#include <BPToolGraphSchemaAction.h>
#include "Core/BoardNode.h"
#include "ToolMenus/Public/ToolMenuSection.h"
#include "ToolMenus/Public/ToolMenu.h"
#include "EdGraph/EdGraphNode.h"
#include "ToolMenus/Public/ToolMenuEntry.h"
#include <BPToolSchemaUtils.h>
#include "SimpleCode.h"

#define LOCTEXT_NAMESPACE "BPToolGraphSchema"

UBPToolGraphSchema::UBPToolGraphSchema(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}


void UBPToolGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TArray<UClass*> CodeClassArray;
	GetDerivedClasses(USimpleCode::StaticClass(), CodeClassArray, false);

	auto Func = [ContextMenuBuilder](const FText& Name, const FText& Category,const FText& Tooltips) -> TSharedPtr<FBPToolGraphSchemaAction>
	{
		TSharedPtr<FBPToolGraphSchemaAction> NewNodeAction(new FBPToolGraphSchemaAction(
			Category,
			Name,
			Tooltips,
			0));
		NewNodeAction->K3Node = NewObject<UBoardNode>(ContextMenuBuilder.OwnerOfTemporaries);
		return NewNodeAction;
	};

	for (UClass* TmpCode : CodeClassArray)
	{
		for (TFieldIterator<UFunction> i(TmpCode); i; ++i)
		{
			TSharedPtr<FBPToolGraphSchemaAction> NewNodeAction = Func(FText::FromString(i->GetName()),FText::FromString(i->GetMetaData("Group")), i->GetToolTipText());
			NewNodeAction->K3Node->Function = *i;
			ContextMenuBuilder.AddAction(NewNodeAction);
		}
		for (TFieldIterator<FProperty> i(TmpCode); i; ++i)
		{
			TSharedPtr<FBPToolGraphSchemaAction> NewNodeAction = Func(FText::FromString(i->GetName()), FText::FromString(i->GetMetaData("Group")), i->GetToolTipText());
			NewNodeAction->K3Node->Property = *i;
			ContextMenuBuilder.AddAction(NewNodeAction);
		}
	}
}

TSharedPtr<FEdGraphSchemaAction> UBPToolGraphSchema::GetCreateCommentAction() const
{
	return TSharedPtr<FEdGraphSchemaAction>(static_cast<FEdGraphSchemaAction*>(new FBPToolGraphSchemaAction));
}


void UBPToolGraphSchema::GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& NewSection = Menu->AddSection("BPToolGraphSchema", LOCTEXT("", ""));
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

	TArray<UClass*> CodeClassArray;
	GetDerivedClasses(USimpleCode::StaticClass(), CodeClassArray, false);

	for (UClass* TmpCode : CodeClassArray)
	{
		for (TFieldIterator<UFunction> i(TmpCode); i; ++i)
		{
			UFunction* Func = *i;
			TSharedPtr<FBPToolGraphSchemaAction> Action = FBPToolSchemaUtils::CreateAction<FBPToolGraphSchemaAction, UBoardNode>(Func->GetName(), Func->GetMetaData("Group"), Func->GetToolTipText().ToString(), OwnerBPGraph);
			Action->K3Node->Function = Func;
			OutActions.Add(Action);
		}

		for (TFieldIterator<FProperty> i(TmpCode); i; ++i)
		{
			FProperty* Prop = *i;
			if (Prop)
			{
				TSharedPtr<FBPToolGraphSchemaAction> Action = FBPToolSchemaUtils::CreateAction<FBPToolGraphSchemaAction, UBoardNode>(Prop->GetName(), Prop->GetMetaData("Group"), Prop->GetToolTipText().ToString(), OwnerBPGraph);
				Action->K3Node->Property = Prop;
				OutActions.Add(Action);
				//FBPToolSchemaUtils::CreateAction<UFunctionGrapNode>(Prop->GetName(), Prop->GetMetaData("Group"), Prop->GetToolTipText(), OwnerBPGraph, OutActions, Prop);
			}
		}
	}
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