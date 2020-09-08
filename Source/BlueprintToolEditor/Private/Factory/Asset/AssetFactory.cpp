#include "Asset/AssetFactory.h"
#include "BlueprintData.h"

UAssetFactory::UAssetFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UBlueprintData::StaticClass();
}

UObject* UAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UBlueprintData>(InParent, InClass, InName, Flags | RF_Transactional);
}

