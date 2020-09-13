#pragma once
#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"

struct FBPToolSchemaUtils
{
	template<typename T, typename TN>
	static TSharedPtr<FEdGraphSchemaAction> CreateAction(FString Title, FString ToolTip, UEdGraph* BPOwner)
	{
		const FText MenuDesc = FText::FromString(Title);
		const FText Category = FText::FromString("BPTool");
		const FText TooltipText = FText::FromString(ToolTip);
		TSharedPtr<T> NewNodeAction(new T(
			Category,
			MenuDesc,
			TooltipText,
			0));
		NewNodeAction->K3Node = NewObject<TN>(BPOwner);
		return NewNodeAction;
	}
};
