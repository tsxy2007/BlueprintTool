#pragma once
#include "CoreMinimal.h"
#include "DescriptionBPTool.generated.h"

USTRUCT(BlueprintType)
struct FDescriptionBPTool
{
	GENERATED_USTRUCT_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
		FTransform MeshTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
		float FloatValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
		FColor ColorValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
		FLinearColor LinearColor;
};