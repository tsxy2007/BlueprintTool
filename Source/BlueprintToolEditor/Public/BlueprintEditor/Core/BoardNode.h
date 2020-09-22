#pragma once
#include "CoreMinimal.h"
#include "K3Node.h"
#include "Details/Blueprint/DescriptionBPTool.h"
#include "BoardNode.generated.h"

struct FDescriptionBPTool;
UCLASS()
class UBoardNode : public UK3Node
{
	GENERATED_BODY()
public:
	UBoardNode() {};
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = K3Node)
		FTransform BoardGrapNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		FDescriptionBPTool BPTool;
};