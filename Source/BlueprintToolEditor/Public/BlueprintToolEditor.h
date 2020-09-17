// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AssetTypeCategories.h"

class FBlueprintToolEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

#if WITH_EDITOR
	virtual EAssetTypeCategories::Type GetAssetCategoryBit() const { return AssetCategoryBit; }
protected:
	EAssetTypeCategories::Type AssetCategoryBit;
#endif
public:
	/** Register custom property type layout */
	void RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate);

	void RegisterPropertyTypeCustomizations();
private:
	TSet< FName > RegisteredPropertyTypes;
private:
	TArray<TSharedPtr<class FAssetTypeActions_Base>> ItemDataAssetTypeActions;
	TSharedPtr<struct FGraphPanelPinConnectionFactory>	GraphPanelPinConnectionFactory;
	TSharedPtr<struct FGraphPanelNodeFactory>			GraphPanelNodeFactory;
	TSharedPtr<struct FGraphPanelPinFactory>				GraphPanelPinFactory;
};
