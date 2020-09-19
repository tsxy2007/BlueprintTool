
#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "Widgets/SWidget.h"

/**
 * Implements a details view customization for the FOpenColorIOColorSpaceConversion
 */
class FBlueprintPropertyTypeCustomization : public IPropertyTypeCustomization
{
public:
	// IPropertyTypeCustomization begin
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	// IPropertyTypeCustomization end

public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
};