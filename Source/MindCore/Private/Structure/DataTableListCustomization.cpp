#include "Structure/DataTableListCustomization.h"
#include "Editor/PropertyEditor/Public/DetailCategoryBuilder.h"
#include "Editor/PropertyEditor/Public/DetailLayoutBuilder.h"

#define LOCTEXT_NAMESPACE "FDataTableListCustomizationLayout"

class SObjectPropertyEntryBox;

void FDataTableListCustomization::CustomizeHeader(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	// this->StructPropertyHandle = InStructPropertyHandle;

	if (InStructPropertyHandle->HasMetaData(TEXT("RowType")))
	{
		const FString& RowType = InStructPropertyHandle->GetMetaData(TEXT("RowType"));
		RowTypeFilter = FName(*RowType);
	}
}

void FDataTableListCustomization::CustomizeChildren(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	IDetailCategoryBuilder &ParentCategory = StructBuilder.GetParentCategory();
	m_LayoutBuilder = &ParentCategory.GetParentLayout();

    m_DataTableHandler = InStructPropertyHandle->GetChildHandle("DataTable");
	m_RowsHandler = InStructPropertyHandle->GetChildHandle("Rows");
	m_RowArrayHandler = m_RowsHandler->AsArray();

    if (m_DataTableHandler->IsValidHandle() && m_RowsHandler->IsValidHandle())
    {
        /** Setup Change callback */
		FSimpleDelegate OnDataTableChangedDelegate = FSimpleDelegate::CreateSP(this, &FDataTableListCustomization::OnDataTableChanged);
		m_DataTableHandler->SetOnPropertyValueChanged(OnDataTableChangedDelegate);

		/** Construct a asset picker widget with a custom filter */
		auto mainName = InStructPropertyHandle->GetPropertyDisplayName();

		m_RowsHandler->SetPropertyDisplayName(FText::FromString(mainName.ToString() + "_Lists"));

		auto mainRow = StructBuilder.AddCustomRow(mainName)
			.NameContent()
			[
				SNew(STextBlock)
				.Text(FText::FromString(mainName.ToString() + "_Table"))
				.Font(StructCustomizationUtils.GetRegularFont())
			]
			.ValueContent()
			.MaxDesiredWidth(0.0f) // don't constrain the combo button width
			[
				SNew(SObjectPropertyEntryBox)
				.PropertyHandle(m_DataTableHandler)
				.AllowedClass(UDataTable::StaticClass())
				.OnShouldFilterAsset(this, &FDataTableListCustomization::ShouldFilterAsset)
			];

		UObject* SourceDataTable;
		if (m_DataTableHandler->GetValue(SourceDataTable) == FPropertyAccess::Success && SourceDataTable)
		{
			TSharedRef<FDetailArrayBuilder> rowsBuilder = MakeShareable(new FDetailArrayBuilder(m_RowsHandler.ToSharedRef()));
			rowsBuilder->OnGenerateArrayElementWidget(FOnGenerateArrayElementWidget::CreateLambda([&](TSharedRef<IPropertyHandle> handler, int32 index, IDetailChildrenBuilder& childBuilder) {
				FName rowName;
				const FPropertyAccess::Result rowResult = handler->GetValue(rowName);
				UComboBoxExtensionHandler* comboHandler = NewObject<UComboBoxExtensionHandler>();
				if (comboHandler)
				{
					comboHandler->m_ComboxIndex = index;
					comboHandler->m_ArrayHandler = m_RowArrayHandler;
					FPropertyComboBoxArgs ComboArgs(m_RowsHandler,
						FOnGetPropertyComboBoxStrings::CreateSP(this, &FDataTableListCustomization::OnGetRowStrings),
						FOnGetPropertyComboBoxValue::CreateUObject(comboHandler, &UComboBoxExtensionHandler::GetCurrentValue),
						FOnPropertyComboBoxValueSelected::CreateUObject(comboHandler, &UComboBoxExtensionHandler::OnGetValue)
					);
					ComboArgs.ShowSearchForItemCount = 1;

					childBuilder.AddProperty(m_RowArrayHandler->GetElement(index))
						.DisplayName(FText::FromString(rowName.ToString()))
						.CustomWidget()
						.NameContent()
						[
							PropertyCustomizationHelpers::MakePropertyComboBox(ComboArgs)
						];
					m_ComboHandlers.Add(comboHandler);
				}
				}));
			StructBuilder.AddCustomBuilder(rowsBuilder);
		}
    }
}

bool FDataTableListCustomization::GetCurrentValue(UDataTable*& OutDataTable, FName& OutName) const
{
	if (m_DataTableHandler.IsValid() && m_DataTableHandler->IsValidHandle())
	{
		// If either handle is multiple value or failure, fail
		UObject* SourceDataTable = nullptr;
		if (m_DataTableHandler->GetValue(SourceDataTable) == FPropertyAccess::Success)
		{
			OutDataTable = dynamic_cast<UDataTable*>(SourceDataTable);

            if (OutDataTable && OutDataTable->FindRow<FTableRowBase>(OutName, TEXT(""), false) != nullptr)
			{
				return true;
			}
		}
	}
	return false;
}


void FDataTableListCustomization::OnGetRowStrings(TArray< TSharedPtr<FString> >& OutStrings, TArray<TSharedPtr<SToolTip>>& OutToolTips, TArray<bool>& OutRestrictedItems) const
{
	UDataTable* DataTable = nullptr;
	FName IgnoredRowName;

	// Ignore return value as we will show rows if table is the same but row names are multiple values
	GetCurrentValue(DataTable, IgnoredRowName);

	TArray<FName> AllRowNames;
	if (DataTable != nullptr)
	{
		for (TMap<FName, uint8*>::TConstIterator Iterator(DataTable->GetRowMap()); Iterator; ++Iterator)
		{
			AllRowNames.Add(Iterator.Key());
		}

		// Sort the names alphabetically.
		AllRowNames.Sort(FNameLexicalLess());
	}

	// Remove item been used
	uint32 count;
	m_RowArrayHandler->GetNumElements(count);
	for (int i = (int)count - 1; i >= 0 ; --i)
	{
		FName value;
		m_RowArrayHandler->GetElement(i)->GetValue(value);
		if (AllRowNames.Contains(value))
		{
			AllRowNames.Remove(value);
		}
	}

	for (const FName& RowName : AllRowNames)
	{
		OutStrings.Add(MakeShared<FString>(RowName.ToString()));
		OutRestrictedItems.Add(false);
	}
}

void FDataTableListCustomization::OnDataTableChanged()
{
	UDataTable* CurrentTable;
	FName OldName;

	uint32 count;
	m_RowArrayHandler->GetNumElements(count);
	for (int i = (int)count - 1; i >= 0 ; --i)
	{
		m_RowArrayHandler->GetElement(i)->GetValue(OldName);
		if (!GetCurrentValue(CurrentTable, OldName))
		{
			m_RowArrayHandler->DeleteItem(i);
			m_ComboHandlers.RemoveAt(i);
		}
	}

	RefreshEditor();
}

bool FDataTableListCustomization::ShouldFilterAsset(const struct FAssetData& AssetData)
{
	if (!RowTypeFilter.IsNone())
	{
		static const FName RowStructureTagName("RowStructure");
		FName RowStructure;
		if (AssetData.GetTagValue<FName>(RowStructureTagName, RowStructure) && RowStructure == RowTypeFilter)
		{
			return false;
		}
		return true;
	}

	return false;
}

void FDataTableListCustomization::RefreshEditor()
{
	if (m_LayoutBuilder)
		m_LayoutBuilder->ForceRefreshDetails();
}

#undef LOCTEXT_NAMESPACE