#pragma once

#include "CoreMinimal.h"
#include "Core/BehaviorExecutorInterface.h"
#include "Core/ActionInterface.h"
#include "Basic/Mind.h"
#include "Structure/ActionInfo.h"
#include "Structure/Thing.h"
#include "Executor.generated.h"

class UMind;
DECLARE_DELEGATE_OneParam(FObtainThingDelegate, const FThing &)
DECLARE_DELEGATE_OneParam(FUseThingDelegate, const FThing &)

USTRUCT(BlueprintType)
struct FWay
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FActionInfo> ActionInfos;

    FWay()
    { }

    FWay(TArray<FActionInfo> hehavior)
        : ActionInfos(hehavior)
    { }

    void Empty()
    {
        ActionInfos.Empty();
    }

    bool HasPrecondition(FDataTableRows& outPrecondition)
    {
        if (ActionInfos.Num() > 0
            && ActionInfos[ActionInfos.Num() - 1].PreconditionTable.Rows.Num() > 0)
        {
            outPrecondition = ActionInfos[ActionInfos.Num() - 1].PreconditionTable;
            return true;
        }

        return false;
    }

    bool GetActionInfo(int index, FActionInfo& outAction)
    {
        if (index < ActionInfos.Num() && index >= 0)
        {
            outAction = ActionInfos[index];
            return true;
        }

        return false;
    }

    bool IsLastAction(int index)
    {
        return index == ActionInfos.Num() - 1;
    }

    bool IsLastActionSequence(int actionIndex, int actionItemIndex)
    {
        if (actionIndex >= ActionInfos.Num())
        {
            UE_LOG(LogTemp, Error, TEXT("Behavior index out of range when tring to find action of target behavior"))
            return true;
        }
        return ActionInfos[actionIndex].ActionSequenceClasses.Num() - 1 == actionItemIndex;
    }

    bool GetActionClass(int actionIndex, int actionItemIndex, TSubclassOf<UObject>& outClass)
    {
        if (actionIndex >=0 && actionIndex < ActionInfos.Num())
        {
            if (actionItemIndex >= 0 && actionItemIndex < ActionInfos[actionIndex].ActionSequenceClasses.Num())
            {
                outClass = ActionInfos[actionIndex].ActionSequenceClasses[actionItemIndex];
                return true;
            }
        }

        return false;
    }

    int GetCost()
    {
        int cost = 0;

        for (int i = 0; i < ActionInfos.Num(); ++i)
        {
            cost += (int)ActionInfos[i].Difficulty;
        }

        return cost;
    }

    friend bool operator==(const FWay& l, const FWay& r)
    {
        return l.ActionInfos == r.ActionInfos;
    }

    void Reverse()
    {
        int count = ActionInfos.Num();
        for (int i = 0; i < count; ++i)
        {
            int reverseIndex = count - i - 1;
            if ( i < reverseIndex)
            {
                auto temp = ActionInfos[i];
                ActionInfos[i] = ActionInfos[reverseIndex];
                ActionInfos[reverseIndex] = temp;
            }
            else
            {
                break;
            }
        }
    }
};

USTRUCT(BlueprintType)
struct FActionData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<TScriptInterface<IActionInterface>> ActionSequence;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int ActionSequenceIndex;

    FActionData()
        : ActionSequence({})
        , ActionSequenceIndex(0)
    {}

    void Reset()
    {
        ActionSequenceIndex = 0;
    }

    void GainActionSequenceIndex()
    {
        ActionSequenceIndex++;
    }

    void AddSequence(const TScriptInterface<IActionInterface>& item)
    {
        ActionSequence.Add(item);
    }

    bool GetCurrentActionSequence(TScriptInterface<IActionInterface>& item)
    {
        if (ActionSequenceIndex >= 0 && ActionSequenceIndex < ActionSequence.Num())
        {
            item = ActionSequence[ActionSequenceIndex];
            return true;
        }

        return false;
    }
};

USTRUCT(BlueprintType) 
struct FExecutorData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
    FThing Target;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FWay Way;
    UPROPERTY(BlueprintReadwrite)
    TArray<FActionData> Actions;
    UPROPERTY(BlueprintReadwrite)
    TArray<TScriptInterface<IActionValidatyInterface>> Validaties;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    EExecutorState State;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int ActionIndex;

    bool GetCurrentAction(FActionData& outAction, bool autoCreate = true)
    {
        if (ActionIndex >= 0 && ActionIndex < Actions.Num())
        {
            outAction = Actions[ActionIndex];
            return true;
        }
        else if (autoCreate && ActionIndex == Actions.Num())
        {
            outAction = FActionData();
            Actions.Add(outAction);
            return true;
        }

        return false;
    }

    bool UpdateCurrentAction(const FActionData& newAction)
    {
        if (ActionIndex >= 0 && ActionIndex < Actions.Num())
        {
            Actions[ActionIndex] = newAction;
            return true;
        }
        
        return false;
    }

    bool GetActionInfo(FActionInfo& outInfo)
    {
        return Way.GetActionInfo(ActionIndex, outInfo);
    }

    bool SwitchAction()
    {
        if (!IsLastAction())
        {
            ActionIndex++;
            return true;
        }

        return false;
    }

    bool IsLastAction()
    {
        return Way.IsLastAction(ActionIndex);
    }

    bool IsLastActionSequence()
    {
        FActionData action;
        if (GetCurrentAction(action, false))
        {
            return Way.IsLastActionSequence(ActionIndex, action.ActionSequenceIndex);
        }

        return false;
    }

    bool GetCurrentReward(TArray<FThing*>& outReward)
    {
        FActionInfo info;
        outReward.Empty();
        if (GetActionInfo(info))
        {
            info.RewardTable.GetRows(TEXT("Get rewards"), outReward);
        }

        return outReward.Num() > 0;
    }
};

UCLASS(Blueprintable)
class MINDCORE_API UExecutor : public UObject, public IBehaviorExecutorInterface
{
    GENERATED_BODY()

public:
    UPROPERTY()
    UMind* m_Mind;
    UPROPERTY(BlueprintReadwrite)
    FExecutorData m_Data;
    UPROPERTY(EditAnywhere)
    UDataTable* m_ActionInfo;
    UPROPERTY(BlueprintReadWrite)
    TArray<FActionInfo> TotalActions;

    FObtainThingDelegate OnObtainThing;
    FUseThingDelegate OnUseThing;

    UFUNCTION(BlueprintNativeEvent)
    void Init(const FThing &thing, UMind* mind);
    virtual void Init_Implementation(const FThing &thing, UMind* mind)
    {
        m_Data.Target = thing;
        m_Data.State = EExecutorState::EXECUTOR_WAITING;
        m_Mind = mind;
        TArray<FActionInfo*> allItems;
        if (m_ActionInfo)
        {
            m_ActionInfo->GetAllRows("Init executor items", allItems);
            for (auto item : allItems)
            {
                TotalActions.Add(*item);
            }
        }
    }

    virtual void Stop();

    UFUNCTION(BlueprintCallable)
    void OwnThing(const FThing &thing)
    {
        OnObtainThing.ExecuteIfBound(thing);
    }

    UFUNCTION(BlueprintCallable)
    void UseThing(const FThing &thing)
    {
        OnUseThing.ExecuteIfBound(thing);
    }

    // Find best way to reach target
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    bool FindWay(FThing target, FWay& ways);
    virtual bool FindWay_Implementation(FThing target, FWay& way);
    // Get all ways of target
    UFUNCTION(BlueprintCallable)
    bool GetAllWays(FThing target, TArray<FWay>& ways);
    // Get all solutions of one target directly
    UFUNCTION(BlueprintCallable)
    bool GetAllSolutions(const FDataTableRows& goals, const FActionInfo &excludeAction, TArray<FActionInfo> &outSolution);
    UFUNCTION(BlueprintCallable)
    bool GetAllSolutionsByThing(const FThing& goal, TArray<FActionInfo>& outSolutions);
    void StashWay(TArray<FWay>& Ways, TArray<FActionInfo> situations, FWay originalGoals = FWay());
    
    UFUNCTION(BlueprintCallable)
    bool CheckPreconditions(const FDataTableRows& precondition, FThing& outTarget);

    #pragma region action
    UFUNCTION(BlueprintCallable)
    bool CreateActionSequence(TScriptInterface<IActionInterface>& action, TSubclassOf<UObject> actionClass);
    UFUNCTION(BlueprintCallable)
    bool BeginSequence(TScriptInterface<IActionInterface>& action, const FThing& precondition);
    FActionInfo* GetActionInfo(UObject* sequence);
    
    void OwnCurrentTarget();
    void UpdateValidaties(const FWay& way);
    void FinishWay();
    #pragma endregion action

    #pragma region difficulty
    UFUNCTION(BlueprintCallable)
    bool UpdateDifficulty(const FString& actionName, EActionDifficulty difficulty);
    void UpdateDifficulty(UObject* targetAction, EActionDifficulty difficulty);
    void SwitchDifficulty(UObject* targetAction, const FString& targetActionName);
    #pragma endregion difficulty

    UFUNCTION(BlueprintCallable)
    bool GetCurrentAction(FActionData& outAction);
    UFUNCTION(BlueprintCallable)
    bool GetCurrentActionInfo(FActionInfo& outInfo);
    UFUNCTION(BlueprintCallable)
    static bool GetActionSequence(FActionData action, TScriptInterface<IActionInterface>& sequence)
    {
        return action.GetCurrentActionSequence(sequence);
    }

#pragma region IExecutor implement
    virtual void CreateBehavior_Implementation() override;
    virtual void UpdateBehavior_Implementation() override;
    virtual void ExecuteBehavior_Implementation() override;
    virtual EExecutorState GetExecuteState_Implementation() override;
    virtual void UpdateState_Implementation(EExecutorState state) override;
#pragma endregion
};