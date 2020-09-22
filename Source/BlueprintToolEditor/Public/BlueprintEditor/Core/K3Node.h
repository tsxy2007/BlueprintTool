#pragma once
#include "CoreMinimal.h"
#include "GraphEditor.h"
#include "UObject/UnrealType.h"
#include "K3Node.generated.h"

UCLASS()
class UK3Node : public UEdGraphNode
{
	GENERATED_BODY()
public:
	UK3Node();

public:
	virtual void AllocateDefaultPins() override;
	FText GetNodeTitleText();

public:
	UPROPERTY()
	UFunction* Function;

	UPROPERTY()
	TFieldPath<FProperty>  Property;
	
	UPROPERTY()
	FText NodeTitle;
};