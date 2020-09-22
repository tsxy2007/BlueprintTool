#pragma once
#include "CoreMinimal.h"
#include "SimpleCode.generated.h"

UCLASS()
class BLUEPRINTTOOLCODE_API USimpleCode : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(Meta=(CodeType="Event",Group="MainEvent"))
	static void EventEntry();

	UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCode"))
		static void PrintString(FString Msg);
};