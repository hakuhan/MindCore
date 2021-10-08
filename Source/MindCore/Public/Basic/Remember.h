/*
    Author: baihan 
    class purpose: Basic remenber 
*/

#pragma once

#include "CoreMinimal.h"
#include "Core/RememberInterface.h"
#include "Core/MindComponentInterface.h"
#include "Structure/MemoryFragment.h"

#include "Remember.generated.h"

UCLASS(Blueprintable)
class MINDCORE_API URemenber : public UObject, public IRememberInterface, public IMindComponentInterface
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly)
    UMind *Mind;
    UPROPERTY(BlueprintReadWrite)
    TArray<FMemoryFragment> m_Memories;


    void UnInstall_Implementation() override;

public:
    virtual bool Remember_Implementation(const FMemoryFragment &target) override;

    virtual bool Remind_Implementation(const FString &thingID, FMemoryFragment &outfragment) override;

    virtual bool Forget_Implementation(const FString &thingID) override;

    virtual bool Share_Implementation(const FString &thingID, const TScriptInterface<IRememberInterface> &target) override;

    virtual void Accept_Implementation(const FString &thingID, const FMemoryFragment &inMemory) override;

    void Clean();

    UFUNCTION(BlueprintCallable)
    int FindIndex(const FString &memoryID);

#pragma region mind component
    virtual void OnInit_Implementation(UMind *mind) override;
#pragma endregion
};
