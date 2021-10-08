/*
    Author: baihan 
    class purpose: Mind imagine can react any part of mind 
*/

#pragma once
#include "ImagineInterface.generated.h"

UINTERFACE()
class MINDCORE_API UImagineInterface : public UInterface
{
    GENERATED_BODY()
};

class MINDCORE_API IImagineInterface
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Imagine")
    void Imaging();
};