#pragma once
#include "EdGraph/EdGraphSchema.h"
#include "BPToolGraphSchemaAction.generated.h"

USTRUCT()
struct BLUEPRINTTOOLEDITOR_API FBPToolGraphSchemaAction : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

	// Simple type info
	static FName StaticGetTypeId() { static FName Type("BPToolGraphSchemaAction"); return Type; }
	virtual FName GetTypeId() const override { return StaticGetTypeId(); }

	FBPToolGraphSchemaAction()
		: FEdGraphSchemaAction()
	{}

	FBPToolGraphSchemaAction(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping)
	{}

	//~ Begin FEdGraphSchemaAction Interface
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	//~ End FEdGraphSchemaAction Interface
};
