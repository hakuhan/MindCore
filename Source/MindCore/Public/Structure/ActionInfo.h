#pragma once

#include "CoreMinimal.h"
#include "Structure/Thing.h"
#include "Core/ActionInterface.h"
#include "Core/BehaviorExecutorInterface.h"
#include "Core/ActionValidatyInterface.h"
#include "Structure/DataTableRows.h"
#include "ActionInfo.generated.h"

USTRUCT(BlueprintType)
struct MINDCORE_API FActionInfo : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadwrite)
    FString Name;
    UPROPERTY(EditAnywhere, BlueprintReadwrite)
    FDataTableRows PreconditionTable;
    UPROPERTY(EditAnywhere, BlueprintReadwrite)
    FDataTableRows RewardTable;
    UPROPERTY(EditAnywhere, BlueprintReadwrite)
    EActionDifficulty Difficulty;
    UPROPERTY(EditAnywhere, BlueprintReadwrite, meta = (MustImplement = "ActionInterface"))
    TArray<TSubclassOf<UObject>> ActionSequenceClasses;
    UPROPERTY(EditAnywhere, BlueprintReadwrite, meta = (MustImplement = "ActionValidatyInterface"))
    TSubclassOf<AActor> ActionValidatyClass;

    friend bool operator==(const FActionInfo& Lhs, const FActionInfo& Rhs)
    {
        return Lhs.Name == Rhs.Name && Lhs.PreconditionTable == Rhs.PreconditionTable && Lhs.RewardTable == Rhs.RewardTable;
    }

    friend bool operator!=(const FActionInfo& Lhs, const FActionInfo& Rhs)
    {
        return !(Lhs == Rhs);
    }

    FActionInfo()
        : Name("Empty")
        , PreconditionTable()
        , RewardTable()
        , Difficulty(EActionDifficulty::BEHAVIOR_NONE)
        , ActionSequenceClasses()
        , ActionValidatyClass()
    { }

    static FActionInfo EmptyBehavior()
    {
        return FActionInfo();
    }

    bool IsNeedsMatched(const FDataTableRows& goal) const
    {
        bool result = false;
        if (RewardTable.DataTable == goal.DataTable)
        {
            for (FName row : RewardTable.Rows)
            {
                if (goal.Rows.Contains(row))
                {
                    result = true;
                    break;
                }
            }
        }

        return result;
    }

    bool IsNeedsMatched(const FName& goal) const
    {
        return RewardTable.Rows.Contains(goal);
    }
};