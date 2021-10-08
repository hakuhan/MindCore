/*
    Author: baihan 
    Date: "2021-03-21" 
    Class discription: Define how to make wishes come true 
*/

#pragma once

#include "Structure/Thing.h"
#include "BehaviorExecutorInterface.generated.h"

UENUM(BlueprintType)
enum class EExecutorState : uint8
{
    EXECUTOR_READY UMETA(DisplayName="Ready"),
    EXECUTOR_EXECUTING UMETA(DisplayName="Executing"),
    EXECUTOR_WAITING UMETA(DisplayName="Waiting"),
    EXECUTOR_FINISH UMETA(DisplayName="Finish"),
};

UINTERFACE(Blueprintable)
class MINDCORE_API UBehaviorExecutorInterface : public UInterface
{
    GENERATED_BODY()
};

class MINDCORE_API IBehaviorExecutorInterface
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mind")
    void CreateBehavior();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mind")
    void UpdateBehavior();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mind")
    void ExecuteBehavior();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mind")
    EExecutorState GetExecuteState();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mind")
    void UpdateState(EExecutorState state);
};