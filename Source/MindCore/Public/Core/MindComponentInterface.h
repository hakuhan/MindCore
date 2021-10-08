#pragma once
#include "MindComponentInterface.generated.h"

class UMind;

UINTERFACE(Blueprintable)
class MINDCORE_API UMindComponentInterface : public UInterface
{
    GENERATED_BODY()
};

class MINDCORE_API IMindComponentInterface
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mind")
    void OnInit(UMind *mind);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mind")
    void UnInstall();
};