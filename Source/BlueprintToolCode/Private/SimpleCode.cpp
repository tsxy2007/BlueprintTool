#include "SimpleCode.h"

void USimpleCode::EventEntry()
{

}

void USimpleCode::PrintString(FString Msg)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, Msg);
	}
}

