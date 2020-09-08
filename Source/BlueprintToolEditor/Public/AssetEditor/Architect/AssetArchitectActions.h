#pragma once

#include "AssetTypeActions_Base.h"

class BLUEPRINTTOOLEDITOR_API FBlueprintTypeActions : public FAssetTypeActions_Base
{
public:
	// IAssetTypeActions Interface Begin
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override;
	virtual void GetActions(const TArray<UObject*>& InObjects, class FMenuBuilder& MenuBuilder) override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual uint32 GetCategories() override;
	// IAssetTypeActions Interface End

};