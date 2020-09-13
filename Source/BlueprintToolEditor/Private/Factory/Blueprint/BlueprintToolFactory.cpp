#include "BlueprintToolFactory.h"
#include "BlueprintData.h"
#include <BPToolGraphSchema.h>
#include "NodeFactory.h"
#include <ConnectionDrawingPolicy1.h>

#define LOCTEXT_NAMESPACE "UBlueprintToolFactory"

UBlueprintToolFactory::UBlueprintToolFactory(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SupportedClass = UBlueprintData::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UBlueprintToolFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class->IsChildOf(UBlueprintData::StaticClass()));
	return NewObject<UBlueprintData>(InParent, Class, Name, Flags);
}

bool UBlueprintToolFactory::CanCreateNew() const
{
	return true;
}

FText UBlueprintToolFactory::GetDisplayName() const
{
	return LOCTEXT("BlueprintToolLabel", "BlueprintTool Theme");
}


/// ///////////////////////////////////////////////////////////////////////////////////////////////

TSharedPtr<class SGraphNode> FBPToolGraphPanelNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	return TSharedPtr<class SGraphNode>();
}

TSharedPtr<class SGraphPin> FBPToolGraphPanelPinFactory::CreatePin(UEdGraphPin* Pin) const
{
	TSharedPtr<class SGraphPin> NewPin;
	if (const UBPToolGraphSchema* K3Scehma = Cast<const UBPToolGraphSchema>( Pin->GetSchema()))
	{
		NewPin = FNodeFactory::CreateK2PinWidget(Pin);
		if (!NewPin.IsValid())
		{
			// TODO:
		}
	}
	return NewPin;
}

FConnectionDrawingPolicy* FBPToolGraphPanelPinConnectionFactory::CreateConnectionPolicy(const UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) const
{
	if (Schema->IsA(UBPToolGraphSchema::StaticClass()))
	{
		return new FBPToolGraphPanelPinConnectionDrawPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	}
	return nullptr;
}



#undef LOCTEXT_NAMESPACE
