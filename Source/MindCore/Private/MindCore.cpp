// Copyright Epic Games, Inc. All Rights Reserved.

#include "MindCore.h"
#include "Structure/DataTableListCustomization.h"
#define LOCTEXT_NAMESPACE "FMindCoreModule"

void FMindCoreModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout( "DataTableRows",                      
        FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDataTableListCustomization::MakeInstance) );

	// This code will execute after your module is loaded into Remember; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogTemp, Warning, TEXT("Startup Mind module!"));
}

void FMindCoreModule::ShutdownModule()
{
	UE_LOG(LogTemp, Warning, TEXT("Shutdown Mind module!"));
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMindCoreModule, MindCore)