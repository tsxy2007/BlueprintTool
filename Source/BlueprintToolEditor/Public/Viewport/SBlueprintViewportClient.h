#pragma once
#include "CoreMinimal.h"
#include "SEditorViewport.h"
#include "AssetArchitectToolkit.h"

class FBlueprintToolEditorToolkit;
class SBlueprintPreviewViewport : public SEditorViewport
{
public:
	SLATE_BEGIN_ARGS(SBlueprintPreviewViewport)
	{}
	SLATE_ARGUMENT(TWeakPtr<FBlueprintToolEditorToolkit>,BPEditorPtr)
	SLATE_ARGUMENT(UBlueprintData*,ObjectToEdit)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	//SEditorViewport interface
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	//End of SEditorViewport interface

private:
	TSharedPtr<class FBPTEditorViewportClient> LevelViewportClient;

	TWeakPtr<FBlueprintToolEditorToolkit> BPEditorPtr;
	UBlueprintData* ObjectToEdit;

	TSharedPtr<class FAdvancedPreviewScene> PreviewScene;

	class USkyLightComponent* Skylight;
	class UAtmosphericFogComponent* AtmosphericFog;
};