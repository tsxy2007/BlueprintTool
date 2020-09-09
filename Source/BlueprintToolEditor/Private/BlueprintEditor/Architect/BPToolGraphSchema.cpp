#include "BPToolGraphSchema.h"

UBPToolGraphSchema::UBPToolGraphSchema(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UBPToolGraphSchema::GetActionList(UEdGraph* OwnerBPGraph, TArray<TSharedPtr<FEdGraphSchemaAction> >& OutActions) const
{
	TSharedPtr<FEdGraphSchemaAction> SchemaAction = MakeShareable(new FEdGraphSchemaAction);
	OutActions.Add(SchemaAction);
}
