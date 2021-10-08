#pragma once

#include "Core/ImagineInterface.h"
#include "Core/MindComponentInterface.h"
#include "Core/BehaviorInterface.h"
#include "Core/WishInterface.h"
#include "Core/RememberInterface.h"
#include "Structure/ActionInfo.h"
#include "Imagine.generated.h"

UCLASS(Blueprintable)
class MINDCORE_API UImagine : public UObject, public IImagineInterface, public IMindComponentInterface
{
    GENERATED_BODY()
public:
    UMind *Mind;

    void Imaging_Implementation();

    UFUNCTION(BlueprintNativeEvent, Category = "Imagine")
    void JoinWish(const TScriptInterface<IWishInterface>& wish);
    void JoinBehavior(const TScriptInterface<IBehaviorInterface>& behavior);
    UFUNCTION(BlueprintNativeEvent, Category = "Imagine")
    void JoinAction(const TScriptInterface<IBehaviorExecutorInterface>&executor ,const TScriptInterface<IActionInterface>& acton, const FActionInfo& actionInfo);
    UFUNCTION(BlueprintNativeEvent, Category = "Imagine")
    void JoinMemory(const TScriptInterface<IRememberInterface>& remember);

#pragma region mind component
    virtual void OnInit_Implementation(UMind *mind) override
    {
        Mind = mind;
    }
#pragma endregion

    void UnInstall_Implementation() override;

};