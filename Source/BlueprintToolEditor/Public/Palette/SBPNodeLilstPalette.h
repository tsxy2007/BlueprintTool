#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "Framework/Commands/InputChord.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "SGraphPalette.h"

class FBlueprintToolEditorToolkit;

//////////////////////////////////////////////////////////////////////////

class SBPNodeLilstPalettePalette : public SGraphPalette
{
	typedef SGraphPalette Super;
public:
	SLATE_BEGIN_ARGS(SBPNodeLilstPalettePalette) {};
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TWeakPtr<FBlueprintToolEditorToolkit> InMaterialEditorPtr);

protected:
	// SGraphPalette Interface
	virtual TSharedRef<SWidget> OnCreateWidgetForAction(FCreateWidgetForActionData* const InCreateData) override;
	virtual void CollectAllActions(FGraphActionListBuilderBase& OutAllActions) override;
	virtual FReply OnActionDragged(const TArray< TSharedPtr<FEdGraphSchemaAction> >& InActions, const FPointerEvent& MouseEvent) override;
	// End of SGraphPalette Interface


protected:
	/** Pointer back to the material editor that owns us */
	TWeakPtr<FBlueprintToolEditorToolkit> BlueprintToolEditorToolkitPtr;
};
