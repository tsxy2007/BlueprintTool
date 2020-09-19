#pragma once

#include "CoreMinimal.h"
#include "BlueprintData.generated.h"

UCLASS()
class BLUEPRINTTOOLRUNTIME_API UBlueprintData : public UObject
{
	GENERATED_BODY()
public:
	UBlueprintData();
	~UBlueprintData();
	static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);
public:
#if WITH_EDITORONLY_DATA

	/** Graph for Behavior Tree */
	UPROPERTY()
		class UEdGraph* BPToolGraph;
#endif
};