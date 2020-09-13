#include <BPToolGraphSchemaAction.h>
#include "Core/BoardNode.h"
#include "UObject/UObjectGlobals.h"

UEdGraphNode* FBPToolGraphSchemaAction::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /*= true*/)
{
	UEdGraphNode* ResultNode = NULL;

	// If there is a template, we actually use it
	if (K3Node != NULL)
	{
		const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "K3_AddNode", "Add Node"));
		ParentGraph->Modify();
		if (FromPin)
		{
			FromPin->Modify();
		}


		UK3Node* Node = DuplicateObject<UK3Node>(K3Node, ParentGraph);
		// set outer to be the graph so it doesn't go away
		Node->Rename(NULL, ParentGraph);
		ParentGraph->AddNode(Node, true, bSelectNewNode);

		Node->CreateNewGuid();
		Node->PostPlacedNewNode();
		Node->AllocateDefaultPins();
		Node->AutowireNewNode(FromPin);

		Node->NodePosX = Location.X;
		Node->NodePosY = Location.Y;
		//@TODO: ANIM: SNAP_GRID isn't centralized or exposed - NodeTemplate->SnapToGrid(SNAP_GRID);

		ResultNode = Node;

		ResultNode->SetFlags(RF_Transactional);

	}

	return ResultNode;
}

