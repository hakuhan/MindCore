/*
    Author: baihan 
    class purpose: Mind memory data 
*/

#pragma once

#include "CoreMinimal.h"
#include "Structure/Thing.h"
#include "MemoryFragment.generated.h"

UENUM(BlueprintType)
enum class EMemoryType : uint8
{
    Memory_Empty UMETA(DisplayName="Empty"),
    Memory_Creating UMETA(DisplayName="Creating"),
    Memory_Happy UMETA(DisplayName="Happy"),
    Memory_Satisfy UMETA(DisplayName="Satisfy"),
    Memory_Insensible UMETA(DisplayName="Insensible"),
    Memory_Contrived UMETA(DisplayName="Contrived"),
    Memory_Sad UMETA(DisplayName="Sad"),
};

USTRUCT(BlueprintType)
struct FMemoryFragment
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
    FString Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
    EMemoryType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
    FThing Target;
};
