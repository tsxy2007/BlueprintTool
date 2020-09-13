#pragma once
#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "EdGraphUtilities.h"
#include "BlueprintToolFactory.generated.h"

UCLASS()
class BLUEPRINTTOOLEDITOR_API UBlueprintToolFactory : public UFactory
{
	GENERATED_UCLASS_BODY()
public:

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override;
	virtual FText GetDisplayName() const override;
};

struct FBPToolGraphPanelNodeFactory : public FGraphPanelNodeFactory
{
public:
	virtual TSharedPtr<class SGraphNode> CreateNode(class UEdGraphNode* Node) const;
};

struct FBPToolGraphPanelPinFactory : public FGraphPanelPinFactory
{
public:
	virtual TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* Pin) const;
};

struct FBPToolGraphPanelPinConnectionFactory : public FGraphPanelPinConnectionFactory
{
public:
	virtual class FConnectionDrawingPolicy* CreateConnectionPolicy(const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;
};