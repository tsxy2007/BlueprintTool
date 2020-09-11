#include <BPToolGraphSchemaAction.h>
#include "Core/BoardNode.h"

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

		// set outer to be the graph so it doesn't go away
		K3Node->Rename(NULL, ParentGraph);
		ParentGraph->AddNode(K3Node, true, bSelectNewNode);

		K3Node->CreateNewGuid();
		K3Node->PostPlacedNewNode();
		K3Node->AllocateDefaultPins();
		K3Node->AutowireNewNode(FromPin);

		K3Node->NodePosX = Location.X;
		K3Node->NodePosY = Location.Y;
		//@TODO: ANIM: SNAP_GRID isn't centralized or exposed - NodeTemplate->SnapToGrid(SNAP_GRID);

		ResultNode = K3Node;

		ResultNode->SetFlags(RF_Transactional);

	}

	return ResultNode;
}

