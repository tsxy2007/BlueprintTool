#include "SK3GrapPin.h"

void SBPToolGraphPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SetCursor(EMouseCursor::Hand);
	IsEditable = true;
	GraphPinObj = InPin;
	check(GraphPinObj != nullptr);
	const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
	check(Schema);
	SBorder::Construct(SBorder::FArguments()
		.BorderBackgroundColor_Lambda([&]() {
			FSlateColor SlateColor(FLinearColor(1.f, 1.f, 1.f));
			return SlateColor;
			})
		.OnMouseButtonDown(this, &SGraphPin::OnPinNameMouseDown)
	)
	;
}

