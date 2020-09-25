#include "SK3Node.h"
#include "Core/K3Node.h"

#define LOCTEXT_NAMESPACE "SK3Node"

void SK3Node::Construct(const FArguments& InArgs, UK3Node* MarkerNode)
{
	GraphNode = MarkerNode;

	this->SetCursor(EMouseCursor::GrabHand);

	this->UpdateGraphNode();
}

void SK3Node::UpdateGraphNode()
{
	SGraphNode::UpdateGraphNode();
	return;
	InputPins.Empty();
	OutputPins.Empty();

	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	const FSlateBrush* MyNodeIcon = FEditorStyle::GetBrush(TEXT("Graph.StateNode.Icon"));

	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(PinBox, SBox)
			.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBorder)
			.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.BorderBackgroundColor_Lambda([&]()
			{
				FSlateColor SlateColor(FLinearColor(1.f, 1.f, 1.f));
				return SlateColor;
			})
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.AutoWidth()
				[
					SAssignNew(LeftNodeBox, SVerticalBox)
				]

			+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Fill)
				.FillWidth(1.0f)
				[
					SAssignNew(RightNodeBox, SVerticalBox)
				]
		]
		]
		];


	CreatePinWidgets();
}

void SK3Node::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	SGraphNode::AddPin(PinToAdd);
	return;
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();

	if (PinObj && PinObj->bAdvancedView)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}

	PinToAdd->SetDesiredSizeScale(FVector2D(16.f, 16.f));

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			.Padding(20.0f, 0.0f)
			[
				PinToAdd
			];

		InputPins.Add(PinToAdd);
	}
	else if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Output)
	{
		RightNodeBox->AddSlot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			.Padding(20.0f, 0.0f)
			[
				PinToAdd
			];

		OutputPins.Add(PinToAdd);
	}
}


#undef LOCTEXT_NAMESPACE