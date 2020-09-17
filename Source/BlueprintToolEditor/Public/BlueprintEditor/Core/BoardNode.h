#pragma once
#include "CoreMinimal.h"
#include "K3Node.h"
#include "BoardNode.generated.h"

UCLASS()
class UBoardNode : public UK3Node
{
	GENERATED_BODY()
public:
	UBoardNode() {};

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = K3Node)
		FTransform BoardGrapNode;
};