/*
    Author: baihan 
    class purpose: Base class of action validaty
*/

#include "Core/ActionValidatyInterface.h"
#include "MindActionValidaty.generated.h"

UCLASS(Blueprintable)
class MINDCORE_API AMindActionValidaty : public AActor, public IActionValidatyInterface
{
    GENERATED_BODY()

public:
    bool CheckActionValidaty_Implementation(AActor* owner) override
    {
        return owner != nullptr;
    }

};