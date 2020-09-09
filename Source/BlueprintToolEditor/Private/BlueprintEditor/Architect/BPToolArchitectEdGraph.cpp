#include "BPToolArchitectEdGraph.h"
#include "BPToolGraphSchema.h"

UBPToolArchitectEdGraph::UBPToolArchitectEdGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Schema = UBPToolGraphSchema::StaticClass();
}

void UBPToolArchitectEdGraph::InitializeGraph()
{

}

