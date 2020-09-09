// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraph.h"
#include "BPToolArchitectEdGraph.generated.h"

UCLASS()
class BLUEPRINTTOOLEDITOR_API UBPToolArchitectEdGraph : public UEdGraph
{
	GENERATED_UCLASS_BODY()
public:
	void InitializeGraph();

};