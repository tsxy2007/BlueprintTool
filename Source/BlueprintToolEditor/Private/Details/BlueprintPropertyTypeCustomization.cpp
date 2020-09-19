#include <BlueprintPropertyTypeCustomization.h>
#include "DetailWidgetRow.h"
#include "IDetailGroup.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyHandle.h"


#define LOCTEXT_NAMESPACE "BlueprintPropertyTypeCustomization"

void FBlueprintPropertyTypeCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HeaderRow
		.WholeRowContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		];
}

void FBlueprintPropertyTypeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	uint32 NumChildren = 0;
	PropertyHandle->GetNumChildren(NumChildren);

	TMap<FString, IDetailGroup*> DetailGroup;

	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
	{
		TSharedRef<IPropertyHandle> ChildHandle = PropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();
		if (ChildHandle->GetProperty())
		{
			if (ChildHandle->GetMetaData("HelloWorld") != "Hide")
			{
				IDetailGroup* DetailGroupInstance = nullptr;
				if (DetailGroup.Contains(ChildHandle->GetMetaData("Category")))
				{
					DetailGroupInstance = DetailGroup[ChildHandle->GetMetaData(TEXT("Category"))];
				}
				else
				{
					DetailGroupInstance = &ChildBuilder.AddGroup(*ChildHandle->GetMetaData(TEXT("Categroy")), FText::Format(LOCTEXT("Catefory1", "{0}"), FText::FromString(*ChildHandle->GetMetaData("Category"))));;
					DetailGroup.Add(ChildHandle->GetMetaData(TEXT("Category")), DetailGroupInstance);
				}
				DetailGroupInstance->AddPropertyRow(ChildHandle);
			}
		}
	}
}

TSharedRef<IPropertyTypeCustomization> FBlueprintPropertyTypeCustomization::MakeInstance()
{
	return MakeShareable(new FBlueprintPropertyTypeCustomization);
}

#undef LOCTEXT_NAMESPACE