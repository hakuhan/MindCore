/*
    Author: baihan 
    class purpose: Remember if Mind 
*/

#pragma once

#include "CoreMinimal.h"

#include "Structure/MemoryFragment.h"
#include "RememberInterface.generated.h"

UINTERFACE()
class MINDCORE_API URememberInterface : public UInterface
{
    GENERATED_BODY()
};

class MINDCORE_API IRememberInterface
{
    GENERATED_BODY()

public:
#pragma region memory loop
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Remember")
    bool Remember(const FMemoryFragment& target);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Remember")
    bool Remind(const FString& thingID, FMemoryFragment &outfragment);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Remember")
    bool Forget(const FString &thingID);
#pragma endregion


    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Remember")
    bool Share(const FString &thingID, const TScriptInterface<IRememberInterface> &target);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Remember")
    void Accept(const FString &thingID, const FMemoryFragment &inMemory);

};