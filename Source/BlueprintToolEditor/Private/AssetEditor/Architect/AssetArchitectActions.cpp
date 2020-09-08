#include "AssetEditor/Architect/AssetArchitectActions.h"
#include "BlueprintData.h"
#include "BlueprintToolEditor.h"
#include "AssetArchitectToolkit.h"

#define LOCTEXT_NAMESPACE "FBlueprintTypeActions"
FText FBlueprintTypeActions::GetName() const
{
	return LOCTEXT("FBlueprintTypeActionsName1","BlueprintTool Theme1");
}

UClass* FBlueprintTypeActions::GetSupportedClass() const
{
	return UBlueprintData::StaticClass();
}

FColor FBlueprintTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

bool FBlueprintTypeActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return FAssetTypeActions_Base::HasActions(InObjects);
}

void FBlueprintTypeActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
}

void FBlueprintTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	//UE4 默认编辑器
	//FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects);
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		auto BlueprintDataAsset = Cast<UBlueprintData>(*ObjIt);
		if (BlueprintDataAsset != nullptr)
		{
			TSharedPtr<FBlueprintToolEditorToolkit> EditorToolkit = MakeShareable(new FBlueprintToolEditorToolkit);
			EditorToolkit->Initialize(BlueprintDataAsset, Mode, EditWithinLevelEditor);
		}
	}
}

uint32 FBlueprintTypeActions::GetCategories()
{
	FBlueprintToolEditorModule& EditorModule = FModuleManager::LoadModuleChecked<FBlueprintToolEditorModule>("BlueprintToolEditor");
	return EditorModule.GetAssetCategoryBit(); 
}

#undef LOCTEXT_NAMESPACE 