#pragma once

#include "SGraphNode.h"

class UK3Node;
class SK3Node : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SK3Node)
	{}
	SLATE_END_ARGS()
public:
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	void Construct(const FArguments& InArgs, UK3Node* InNode);
	virtual void UpdateGraphNode() override;
protected:
	TSharedPtr<class SBox> PinBox;
};