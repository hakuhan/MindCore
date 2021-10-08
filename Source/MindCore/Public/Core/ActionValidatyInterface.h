/*
    Author: baihan 
    class purpose: Validate action
*/

#pragma once

#include "ActionValidatyInterface.generated.h"

UINTERFACE(Blueprintable)
class MINDCORE_API UActionValidatyInterface : public UInterface
{
    GENERATED_BODY()
};

class MINDCORE_API IActionValidatyInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Action Validaty")
    bool CheckActionValidaty(AActor* owner);
};
