#pragma once
#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "BlueprintToolFactory.generated.h"

UCLASS()
class BLUEPRINTTOOLEDITOR_API UBlueprintToolFactory : public UFactory
{
	GENERATED_UCLASS_BODY()
public:

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override;
	virtual FText GetDisplayName() const override;
};