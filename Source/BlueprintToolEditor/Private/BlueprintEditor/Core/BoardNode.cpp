#include "Core/BoardNode.h"

#define LOCTEXT_NAMESPACE "BoardNode"
void UBoardNode::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
	CreatePin(EGPD_Input, TEXT("float"), "");
	CreatePin(EGPD_Output, TEXT("float"), "");
}

FText UBoardNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("BoardNode","测试节点");
}

#undef LOCTEXT_NAMESPACE