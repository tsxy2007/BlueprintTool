#pragma once
#include "Toolkits/AssetEditorToolkit.h"

class UBlueprintData;
class FTabManager;
class SBlueprintPreviewViewport;
class SGraphEditor;


class BLUEPRINTTOOLEDITOR_API FBlueprintToolEditorToolkit : public FAssetEditorToolkit, public FNotifyHook
{
	typedef FAssetEditorToolkit Super;
public:
	// FAssetEditorToolkit Interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	virtual void Initialize(UBlueprintData* InTextAsset, const EToolkitMode::Type InMode, const TSharedPtr<IToolkitHost>& InToolkitHost);
	const TSharedPtr<SGraphEditor> GetGraphEditor() { return GraphEditor; };
protected:
	TSharedRef<SDockTab> HandleTabManagerSpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier);
	TSharedRef<SGraphEditor> CreateBPGraphEditor(UEdGraph* InGraph);
private:
	UBlueprintData* BlueprintData;
	TSharedPtr<SBlueprintPreviewViewport> PreviewViewport;
	TSharedPtr<SGraphEditor> GraphEditor;
};