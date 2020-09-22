#pragma once

#include "CoreMinimal.h"
#include "SimpleCode.h"
#include "SimpleCodeLibrary.generated.h"

class UStaticMesh;
UCLASS()
class BLUEPRINTTOOLCODE_API USimpleCodeLibrary : public USimpleCode
{
	GENERATED_BODY()
public:
	UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
	static void Hello1(UObject* Context, int32& A, float B, bool C);


	UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
	static void Hello2(UObject* Contex);


	UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
	static FString Hello3(UObject* Context);

	UPROPERTY(Meta = (CodeType = "Resource", Group = "SimpleCodeLibrary"))
		TSubclassOf<UStaticMesh> Mesh;
};