#include "Commands/BPToolCommands.h"


#define LOCTEXT_NAMESPACE "BPToolCommands"

/** UI_COMMAND takes long for the compile to optimize */
PRAGMA_DISABLE_OPTIMIZATION
void FBPToolCommands::RegisterCommands()
{
	UI_COMMAND(CompileBlueprint, "Compile", "Compile the blueprint", EUserInterfaceActionType::Button, FInputChord(EKeys::F7));
}


PRAGMA_ENABLE_OPTIMIZATION

#undef LOCTEXT_NAMESPACE