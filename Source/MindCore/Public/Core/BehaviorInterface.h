#pragma once

#include "BehaviorInterface.generated.h"

UENUM(BlueprintType)
enum class EBehaviorState : uint8
{
    BEHAVIOR_READY UMETA(DisplayName = "Ready"),
    BEHAVIOR_EXECUTING UMETA(DisplayName = "Executing"),
    BEHAVIOR_FINISH UMETA(DisplayName = "Finish"),
};

UINTERFACE(Blueprintable)
class MINDCORE_API UBehaviorInterface : public UInterface
{
    GENERATED_BODY()
};

class MINDCORE_API IBehaviorInterface
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mind")
    void Behave();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mind")
    EBehaviorState GetState();
};