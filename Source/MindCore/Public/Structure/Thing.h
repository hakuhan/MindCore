/*
    Author: baihan 
    class purpose: Mark for every thing that can be recognized by Mind
*/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Thing.generated.h"

USTRUCT(BlueprintType)
struct MINDCORE_API FThing : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thing")
    FString Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thing")
    FString Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thing")
    int Number;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thing")
    bool NeverEnding;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thing")
    FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thing")
    int Priority;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thing")
    bool Disappearance;

    FThing()
        : Name("Nothing")
    {
    }

    static const FThing EmptyThing()
    {
        return FThing();
    }

    friend bool operator==(const FThing& Lhs, const FThing& Rhs)
    {
        return Lhs.Name == Rhs.Name && Lhs.Type == Rhs.Type;
    }

    friend bool operator!=(const FThing& Lhs, const FThing& Rhs)
    {
        return !(Lhs == Rhs);
    }
};