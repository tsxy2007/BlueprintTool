#include "Core/BoardNode.h"
#include "Details/Blueprint/DescriptionBPTool.h"

#define LOCTEXT_NAMESPACE "BoardNode"
void UBoardNode::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
	CreatePin(EGPD_Input, TEXT("float"), "in");
	CreatePin(EGPD_Output, TEXT("float"), "out");

	CreatePin(EGPD_Input, TEXT("int"), "in");
	CreatePin(EGPD_Output, TEXT("int"), "out");

	CreatePin(EGPD_Input, TEXT("bool"), "in");
	CreatePin(EGPD_Output, TEXT("bool"), "out");
}

FText UBoardNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("BoardNode","测试节点");
}

#undef LOCTEXT_NAMESPACE