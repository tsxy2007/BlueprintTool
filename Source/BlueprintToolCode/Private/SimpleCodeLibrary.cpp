#include "SimpleCodeLibrary.h"

void USimpleCodeLibrary::Hello1(UObject* Context, int32& A, float B, bool C)
{
	PrintString("Hello1");
}

void USimpleCodeLibrary::Hello2(UObject* Contex)
{
	PrintString("Hello2");
}

FString USimpleCodeLibrary::Hello3(UObject* Context)
{
	PrintString("Hello3");
	return TEXT("TTTT");
}

