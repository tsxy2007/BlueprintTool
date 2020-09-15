#pragma once
#include "SGraphPin.h"

class BLUEPRINTTOOLEDITOR_API SBPToolGraphPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SBPToolGraphPin) {	}
	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);
	TSharedPtr<SGraphNode> GetGraphNode() { return OwnerNodePtr.Pin(); }
};