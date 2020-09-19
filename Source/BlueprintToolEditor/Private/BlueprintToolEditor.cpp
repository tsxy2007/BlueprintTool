// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BlueprintToolEditor.h"
#include "AssetToolsModule.h"
#include "AssetArchitectActions.h"
#include <BlueprintToolFactory.h>
#include "EdGraphUtilities.h"
#include <BlueprintDetailCustomization.h>
#include <BlueprintPropertyTypeCustomization.h>

#define LOCTEXT_NAMESPACE "FBlueprintToolEditorModule"

void FBlueprintToolEditorModule::StartupModule()
{

	IAssetTools& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	TSharedPtr<FBlueprintTypeActions> BlueprintAssetTypeAction = MakeShareable(new FBlueprintTypeActions);
	AssetToolsModule.RegisterAssetTypeActions(BlueprintAssetTypeAction.ToSharedRef());
	ItemDataAssetTypeActions.AddUnique(BlueprintAssetTypeAction);
	if (GIsEditor)
	{
		// register AI category so that AI assets can register to it
		AssetCategoryBit = AssetToolsModule.RegisterAdvancedAssetCategory(FName(TEXT("Custom")), LOCTEXT("FBlueprintToolEditorModuleAssetCategory", "Custom"));
	}
	GraphPanelPinConnectionFactory = MakeShareable(new FBPToolGraphPanelPinConnectionFactory);
	GraphPanelNodeFactory = MakeShareable(new FBPToolGraphPanelNodeFactory);
	GraphPanelPinFactory = MakeShareable(new FBPToolGraphPanelPinFactory);
	FEdGraphUtilities::RegisterVisualPinConnectionFactory(GraphPanelPinConnectionFactory);
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory);
	FEdGraphUtilities::RegisterVisualPinFactory(GraphPanelPinFactory);


	
}

void FBlueprintToolEditorModule::ShutdownModule()
{
	FEdGraphUtilities::UnregisterVisualPinConnectionFactory(GraphPanelPinConnectionFactory);
	FEdGraphUtilities::UnregisterVisualNodeFactory(GraphPanelNodeFactory);
	FEdGraphUtilities::UnregisterVisualPinFactory(GraphPanelPinFactory);
	if (ItemDataAssetTypeActions.Num())
	{
		if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			IAssetTools& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

			for (const auto& Item : ItemDataAssetTypeActions)
			{
				AssetToolsModule.UnregisterAssetTypeActions(Item.ToSharedRef());
			}
		}
		ItemDataAssetTypeActions.Empty();
	}

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	for (auto It = RegisteredPropertyTypes.CreateConstIterator(); It; ++It)
	{
		if (It->IsValid())
		{
			PropertyModule.UnregisterCustomPropertyTypeLayout(*It);
		}
	}

	PropertyModule.NotifyCustomizationModuleChanged();
}


void FBlueprintToolEditorModule::RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate)
{
	check(PropertyTypeName != NAME_None);

	RegisteredPropertyTypes.Add(PropertyTypeName);

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(PropertyTypeName, PropertyTypeLayoutDelegate);
}


void FBlueprintToolEditorModule::RegisterPropertyTypeCustomizations()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	RegisterCustomPropertyTypeLayout("DescriptionBPTool", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FBlueprintVariableMappings::MakeInstance));
	//RegisterCustomPropertyTypeLayout("BlueprintPropertyType", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FBlueprintPropertyTypeCustomization::MakeInstance));
	PropertyModule.NotifyCustomizationModuleChanged();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBlueprintToolEditorModule, BlueprintToolEditor)