#include "BlueprintData.h"

UBlueprintData::UBlueprintData()
	:BPToolGraph(nullptr)
{

}


UBlueprintData::~UBlueprintData()
{

}

void UBlueprintData::AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector)
{
#if WITH_EDITORONLY_DATA

	Collector.AddReferencedObject(CastChecked<UBlueprintData>(InThis)->BPToolGraph, CastChecked<UBlueprintData>(InThis));

#endif	// WITH_EDITORONLY_DATA

	Super::AddReferencedObjects(InThis, Collector);
}

