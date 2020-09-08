#include "BlueprintToolFactory.h"
#include "BlueprintData.h"

#define LOCTEXT_NAMESPACE "UBlueprintToolFactory"

UBlueprintToolFactory::UBlueprintToolFactory(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SupportedClass = UBlueprintData::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UBlueprintToolFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class->IsChildOf(UBlueprintData::StaticClass()));
	return NewObject<UBlueprintData>(InParent, Class, Name, Flags);
}

bool UBlueprintToolFactory::CanCreateNew() const
{
	return true;
}

FText UBlueprintToolFactory::GetDisplayName() const
{
	return LOCTEXT("BlueprintToolFactory","BlueprintData");
}

#undef LOCTEXT_NAMESPACE