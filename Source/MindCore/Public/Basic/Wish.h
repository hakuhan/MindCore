/*
    Author: baihan 
    Date: "2021-03-21" 
    Class discription: Base class creating goal for AI 
*/

#pragma once

#include "CoreMinimal.h"

#include "Core/WishInterface.h"
#include "Core/MindComponentInterface.h"
#include "Structure/Thing.h"
#include "Wish.generated.h"

DECLARE_DELEGATE_OneParam(FOnUpdateThingDelegate, const FThing &)

USTRUCT(BlueprintType)
struct MINDCORE_API FWishInfo
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FThing> Wishes;

    void Empty()
    {
        Wishes.Empty();
    }
};

USTRUCT(BlueprintType)
struct FWishData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MindCore)
    TArray<FThing> OwnedThings;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MindCore)
    TArray<FThing> SeekingWishes;

    void Empty()
    {
        OwnedThings.Empty();
        SeekingWishes.Empty();
    }
};

UCLASS(Blueprintable)
class MINDCORE_API UWish : public UObject, public IWishInterface, public IMindComponentInterface
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly)
    UMind *Mind;
    UPROPERTY(BlueprintReadWrite)
    FWishInfo m_Info;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FWishData m_Data;
    
    FOnUpdateThingDelegate OnUpdateThing;

    UFUNCTION(BlueprintCallable)
    void Init(const FWishInfo &Info);
    void Reset();

#pragma region wish interface
    virtual bool CreateWish_Implementation(const FThing &wish) override;
    virtual void UpdateWish_Implementation() override;
    virtual bool LoseWish_Implementation(const FThing &wish) override;
    virtual bool ObtainThing_Implementation(const FThing &thing) override;
    virtual bool LoseThing_Implementation(const FThing &thing) override;
    virtual bool GetWishes_Implementation(TArray<FThing> &wishes) override;
    virtual bool CheckThingOwned_Implementation(const FThing& thing) override;
    virtual bool UpdatePriority_Implementation(const FThing& target, int priority) override;

#pragma endregion

#pragma region mind component
    virtual void OnInit_Implementation(UMind *mind) override;
#pragma endregion

    void UnInstall_Implementation() override;

};