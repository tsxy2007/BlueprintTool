#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraphSchema.h"
#include "BPToolGraphSchema.generated.h"

class UEdGraph;

UCLASS(MinimalAPI)
class UBPToolGraphSchema : public UEdGraphSchema
{
	GENERATED_UCLASS_BODY()

public:
	void GetActionList(UEdGraph* OwnerBPGraph, TArray<TSharedPtr<FEdGraphSchemaAction> >& OutActions) const;
};