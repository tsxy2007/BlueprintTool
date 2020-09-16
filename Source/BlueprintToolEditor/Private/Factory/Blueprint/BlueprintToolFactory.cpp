#include "BlueprintToolFactory.h"
#include "BlueprintData.h"
#include <BPToolGraphSchema.h>
#include "NodeFactory.h"
#include <ConnectionDrawingPolicy1.h>
#include "Core/BoardNode.h"
#include "SK3Node.h"
#include "SK3GrapPin.h"
#include "KismetPins/SGraphPinNum.h"
#include "KismetPins/SGraphPinBool.h"
#include "KismetPins/SGraphPinInteger.h"

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
	if (Node->IsA<UBoardNode>())
	{
		return SNew(SK3Node,Cast<UBoardNode>(Node));
	}
	return TSharedPtr<class SGraphNode>();
}

TSharedPtr<class SGraphPin> FBPToolGraphPanelPinFactory::CreatePin(UEdGraphPin* InPin) const
{
	if (const UBPToolGraphSchema* K3Scehma = Cast<const UBPToolGraphSchema>(InPin->GetSchema()))
	{
		if (InPin->PinType.PinCategory == TEXT("float"))
		{
			return SNew(SGraphPinNum<float>, InPin);
		}
		else if (InPin->PinType.PinCategory == TEXT("bool"))
		{
			return SNew(SGraphPinBool, InPin);
		}
		else if (InPin->PinType.PinCategory == TEXT("int"))
		{
			return SNew(SGraphPinInteger, InPin);
		}
		else if (InPin->PinType.PinCategory == TEXT("int64"))
		{
			return SNew(SGraphPinNum<int64>, InPin);
		}
		return SNew(SBPToolGraphPin, InPin);
	}
	return nullptr;
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
