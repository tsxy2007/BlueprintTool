#pragma once

#include "CoreMinimal.h"
#include "EditorStyleSet.h"
#include "Framework/Commands/Commands.h"

class FUICommandInfo;
//////////////////////////////////////////////////////////////////////////
// FBPToolCommands

/** Set of kismet 2 wide commands */
class FBPToolCommands : public TCommands<FBPToolCommands>
{
public:
	FBPToolCommands()
		: TCommands<FBPToolCommands>(TEXT("BPToolEditor"), NSLOCTEXT("Contexts", "BPToolEditor", "BPTool Editor"), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

public:

	// File-ish commands
	TSharedPtr< FUICommandInfo > CompileBlueprint;

};