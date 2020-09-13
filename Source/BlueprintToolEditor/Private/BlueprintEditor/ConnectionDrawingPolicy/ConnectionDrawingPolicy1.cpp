#include <ConnectionDrawingPolicy1.h>

FBPToolGraphPanelPinConnectionDrawPolicy::FBPToolGraphPanelPinConnectionDrawPolicy(int32 InBackLayerID, int32 InFrontLayerID,
	float InZoomFactor, 
	const FSlateRect& InClippingRect, 
	FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj)
	: FConnectionDrawingPolicy(InBackLayerID,InFrontLayerID,InZoomFactor,InClippingRect,InDrawElements)
{

}

void FBPToolGraphPanelPinConnectionDrawPolicy::DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, /*inout*/ FConnectionParams& Params)
{
	Params.WireThickness = 0.5f;
	Params.WireColor = FLinearColor::Red;
	if (HoveredPins.Num() > 0)
	{
		ApplyHoverDeemphasis(OutputPin, InputPin, Params.WireThickness, Params.WireColor);
	}
}

