#pragma once

#include "CoreMinimal.h"
#include "Editor/PropertyEditor/Public/IPropertyTypeCustomization.h"
#include "Editor/PropertyEditor/Public/IDetailChildrenBuilder.h"
#include "Editor/PropertyEditor/Public/DetailWidgetRow.h"
#include "Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h"
#include "DataTableRows.h"

class FDataTableListCustomization : public IPropertyTypeCustomization
{
public:

    static TSharedRef<IPropertyTypeCustomization> MakeInstance()
    {
        return MakeShareable( new FDataTableListCustomization );
    }

    /** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
    bool ShouldFilterAsset(const struct FAssetData& AssetData);
    bool GetCurrentValue(UDataTable*& OutDataTable, FName& OutName) const;
    void OnGetRowStrings(TArray< TSharedPtr<FString> >& OutStrings, TArray<TSharedPtr<SToolTip>>& OutToolTips, TArray<bool>& OutRestrictedItems) const;
    void OnDataTableChanged();
    void RefreshEditor();

private:
    TSharedPtr<IPropertyHandle> m_DataTableHandler;
    TSharedPtr<IPropertyHandle> m_RowsHandler;
    TSharedPtr<IPropertyHandleArray> m_RowArrayHandler;
    IDetailLayoutBuilder* m_LayoutBuilder;
    TArray<UComboBoxExtensionHandler*> m_ComboHandlers;
    /** The MetaData derived filter for the row type */
	FName RowTypeFilter;
};