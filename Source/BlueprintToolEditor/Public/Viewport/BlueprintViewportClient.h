#pragma once

#include "CoreMinimal.h"
#include "Misc/Guid.h"
#include "InputCoreTypes.h"
#include "UnrealWidget.h"
#include "EditorViewportClient.h"
#include "Toolkits/AssetEditorToolkit.h"

class UBlueprintData;
class SBlueprintPreviewViewport;

class BLUEPRINTTOOLEDITOR_API FBPTEditorViewportClient : public FEditorViewportClient, public TSharedFromThis<FBPTEditorViewportClient>
{
public:
	FBPTEditorViewportClient(
		TWeakPtr<FAssetEditorToolkit> InBlueprintEditor,
		TWeakPtr<SBlueprintPreviewViewport> InBlueprintEditorViewport,
		FPreviewScene& InPreviewScene,
		UBlueprintData* InData);

	virtual void Tick(float DeltaSeconds) override;
};