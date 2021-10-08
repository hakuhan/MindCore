/*
    Author: baihan 
    class purpose: Main Mind interface 
*/

#pragma once
#include "Core/MindComponentInterface.h"
#include "MindInterface.generated.h"

UENUM(BlueprintType)
enum class EMindComponentType : uint8
{
	WISH_COMP = 0 UMETA(DisplayName="Wish"),
	BEHAVIOR_COMP UMETA(DisplayName="Behavior"),
    IMAGINE_COMP UMETA(DisplayName="Imagine"),
    REMEMBER_COMP UMETA(DisplayName="Remember")
};


UINTERFACE(Blueprintable)
class MINDCORE_API UMindInterface : public UInterface
{
    GENERATED_BODY()
};

class MINDCORE_API IMindInterface
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mind")
    void DoWish();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mind")
    void DoBehavior();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mind")
    void DoImagine();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mind")
    void GetRemember(TScriptInterface<IRememberInterface>& outRemember);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mind")
    void UpdateComponent(EMindComponentType type, TSubclassOf<UObject> newComponent);
};