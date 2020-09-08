#pragma once
#include "Toolkits/AssetEditorToolkit.h"

class UBlueprintData;
class FTabManager;
class SBlueprintPreviewViewport;
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

protected:
	TSharedRef<SDockTab> HandleTabManagerSpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier);
private:
	UBlueprintData* BlueprintData;
	TSharedPtr<SBlueprintPreviewViewport> PreviewViewport;
};