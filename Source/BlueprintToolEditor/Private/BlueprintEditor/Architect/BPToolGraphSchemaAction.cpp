#include <BPToolGraphSchemaAction.h>

UEdGraphNode* FBPToolGraphSchemaAction::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /*= true*/)
{

	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "BPEditorNewExpression", "BPTool Editor: New Expression"));

	//UBlueprintData* NewExpression = BPToolSchemaUtils::CreateNewMaterialExpression(ParentGraph, MaterialExpressionClass, Location, bSelectNewNode, /*bAutoAssignResource*/true);

	//if (NewExpression)
	//{
	//	if (MaterialExpressionClass == UMaterialExpressionFunctionInput::StaticClass() && FromPin)
	//	{
	//		// Set this to be an input of the type we dragged from
	//		SetFunctionInputType(CastChecked<UMaterialExpressionFunctionInput>(NewExpression), UMaterialGraphSchema::GetMaterialValueType(FromPin));
	//	}

		//NewExpression->GraphNode->AutowireNewNode(FromPin);

		//return NewExpression->GraphNode;
	//}

	return NULL;
}

