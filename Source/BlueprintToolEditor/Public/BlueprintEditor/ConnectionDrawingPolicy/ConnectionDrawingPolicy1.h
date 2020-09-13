#pragma once
#include "ConnectionDrawingPolicy.h"

class FBPToolGraphPanelPinConnectionDrawPolicy : public FConnectionDrawingPolicy
{
public:
	FBPToolGraphPanelPinConnectionDrawPolicy(int32 InBackLayerID,
		int32 InFrontLayerID, 
		float InZoomFactor, 
		const FSlateRect& InClippingRect,
		FSlateWindowElementList& InDrawElements,
		UEdGraph* InGraphObj);

	virtual void DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, /*inout*/ FConnectionParams& Params) override;

};