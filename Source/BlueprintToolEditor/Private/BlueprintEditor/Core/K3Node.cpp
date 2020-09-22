#include "Core/K3Node.h"
#include "Type/BTEType.h"

#define LOCTEXT_NAMESPACE "K3Node"
UK3Node::UK3Node()
{

}

void UK3Node::AllocateDefaultPins()
{
	if (Function)
	{
		CreatePin(EGPD_Output, FPC_Public::PC_Exec, FName(), FPC_Public::PC_Exec);
		if (Function->GetMetaData("CodeType")!= "Event")
		{
			CreatePin(EGPD_Input, FPC_Public::PC_Exec, FName(), FPC_Public::PC_Exec);
			
			FField* Prop = Function->ChildProperties;
			while (Prop != NULL)
			{
				FProperty* MyProperty = CastField<FProperty>(Prop);
				if (MyProperty->PropertyFlags&CPF_OutParm || MyProperty->PropertyFlags& CPF_ReturnParm)
				{
					CreatePin(EGPD_Output, MyProperty->GetFName(), FName(), MyProperty->GetFName());
				}
				else if (MyProperty->PropertyFlags & CPF_Parm)
				{
					CreatePin(EGPD_Input, MyProperty->GetFName(), FName(), MyProperty->GetFName());
				}
				Prop = Prop->Next;
			}
		}
	}
	else if(Property.Get())
	{
		CreatePin(EGPD_Output, Property->GetFName(), FName(), Property->GetFName());
	}
}

FText UK3Node::GetNodeTitleText()
{
	if (!NodeTitle.IsEmpty())
	{
		return NodeTitle;
	}
	if (Function)
	{
		NodeTitle = FText::FromString(Function->GetName());
	}
	else if (Property.Get())
	{
		NodeTitle = FText::FromString(Property->GetName());
	}
	return NodeTitle;
}
#undef LOCTEXT_NAMESPACE

