/*
    Author: baihan 
    class purpose: execute unit 
*/

#pragma once

#include "ActionInterface.generated.h"

class UImagine;

DECLARE_DELEGATE_TwoParams(FUpdateDifficultyDelegate, UObject*, EActionDifficulty)
DECLARE_DELEGATE_TwoParams(FSwitchDifficultyDelegate, UObject*, const FString&)

UENUM(BlueprintType)
enum class EActionDifficulty : uint8
{
    BEHAVIOR_NONE = 0 UMETA(DisplayName="None", Hidden),
    BEHAVIOR_EASY UMETA(DisplayName="Easy"),
    BEHAVIOR_NORMAL UMETA(DisplayName="Normal"),
    BEHAVIOR_HARD UMETA(DisplayName="Hard"),
    BEHAVIOR_DIFFICULT UMETA(DisplayName="Difficult"),
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
    Action_Inactive UMETA(DisplayName = "Inactive"),
    Action_Unreachable UMETA(DisplayName = "unreachable"),
    Action_Active UMETA(DisplayName = "Active"),
    Action_Success UMETA(DisplayName = "Success"),
    Action_Failure UMETA(DisplayName = "Failure"),
};

UINTERFACE(Blueprintable)
class MINDCORE_API UActionInterface : public UInterface
{
    GENERATED_BODY()
};

class MINDCORE_API IActionInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
    void Init(AActor* owner);
    // does this action cost conditions 
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
    bool IsCost();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
    void PrepareAction();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
    void RunningAction();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
    void FinishAction();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
    void End();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
    EActionState GetState();

    // Efficay check
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
    bool CanEfficacyLose();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
    bool CheckLose();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
    void OnLose();

    // Difficulty
    virtual FUpdateDifficultyDelegate& GetDifficultyDelegate() = 0;
    virtual FSwitchDifficultyDelegate& GetSwitchDifficultyDelegate() = 0;
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
    void UpdateDifficulty(EActionDifficulty difficulty);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
    void SwitchDifficulty(const FString& targetActionName);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DeterminesOutputType = "imagineClass"), Category = "Action")
    UObject* GetImagine(TSubclassOf<UImagine> imagineClass);
};