#pragma once
#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"

struct FBPToolSchemaUtils
{
	template<typename T, typename TN>
	static TSharedPtr<T> CreateAction(FString Title,const FString InCategory, FString ToolTip, UEdGraph* BPOwner)
	{
		const FText MenuDesc = FText::FromString(Title);
		const FText Category = InCategory.IsEmpty() ? FText::FromString("BPTool") : FText::FromString(InCategory);
		const FText TooltipText = FText::FromString(ToolTip);
		TSharedPtr<T> NewNodeAction(new T(Category,MenuDesc,TooltipText,0));
		NewNodeAction->K3Node = NewObject<TN>(BPOwner);
		return NewNodeAction;
	}
};
