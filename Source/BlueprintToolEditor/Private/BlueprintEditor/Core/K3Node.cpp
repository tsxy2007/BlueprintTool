#include "Core/K3Node.h"
#include "Type/BTEType.h"
#include "UObject/UnrealType.h"

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
					CreatePinType(MyProperty, EGPD_Output);
					//CreatePin(EGPD_Output, MyProperty->GetFName(), FName(), MyProperty->GetFName());
				}
				else if (MyProperty->PropertyFlags & CPF_Parm)
				{

					CreatePinType(MyProperty, EGPD_Input);
					//CreatePin(EGPD_Input, MyProperty->GetFName(), FName(), MyProperty->GetFName());
				}
				Prop = Prop->Next;
			}
		}
	}
	else if(Property.Get())
	{
		//CreatePin(EGPD_Output, Property->GetFName(), FName(), Property->GetFName());
		CreatePinType(Property.Get(),EGPD_Output);
	}
}


FText UK3Node::GetNodeTitle(ENodeTitleType::Type Type) const
{
	UK3Node* Node = const_cast<UK3Node*>(this);
	return Node->GetNodeTitleText();
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

void UK3Node::CreatePinType(FProperty* MyProperty, EEdGraphPinDirection EdGraphPinDirection)
{
	if (MyProperty->IsA<FBoolProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Boolean, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FIntProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Int, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FInt64Property>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Int64, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FStructProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Struct, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FEnumProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Enum, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FTextProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Text, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FStrProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_String, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FClassProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Class, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FFloatProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Float, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FByteProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Byte, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FNameProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Name, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FDelegateProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Delegate, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FObjectProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Object, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FInterfaceProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Interface, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FFieldPathProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_FieldPath, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FSoftClassProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_SoftObject, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA<FSoftObjectProperty>())
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_SoftClass, FName(), MyProperty->GetFName());
	}
	else
	{
		CreatePin(EdGraphPinDirection, Property->GetFName(), FName(), Property->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE

