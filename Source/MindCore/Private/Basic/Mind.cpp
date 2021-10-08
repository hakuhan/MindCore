#include "Basic/Mind.h"
#include "Basic/Wish.h"
#include "Basic/Behavior.h"
#include "Basic/Remember.h"
#include "Basic/Imagine.h"

UMind::UMind()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UMind::BeginPlay()
{
	Super::BeginPlay();

	auto rememberObj = NewObject<UObject>(this, RememberClass);
	auto wishObj = NewObject<UObject>(this, WishClass);
	auto behaviorObj = NewObject<UObject>(this, BehaviorClass);
	auto imagineObj = NewObject<UObject>(this, ImagineClass);

	if (rememberObj == nullptr || wishObj == nullptr || behaviorObj == nullptr || imagineObj == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Mind should be initialized!"));
	}
	else
	{
		Remember.SetObject(rememberObj);
		Remember.SetInterface(dynamic_cast<IRememberInterface*>(rememberObj));

		Wish.SetObject(wishObj);
		Wish.SetInterface(dynamic_cast<IWishInterface*>(wishObj));

		Behavior.SetObject(behaviorObj);
		Behavior.SetInterface(dynamic_cast<IBehaviorInterface*>(behaviorObj));

		Imagine.SetObject(imagineObj);
		Imagine.SetInterface(dynamic_cast<IImagineInterface*>(imagineObj));
		IMindComponentInterface::Execute_OnInit(wishObj, this);
		IMindComponentInterface::Execute_OnInit(rememberObj, this);
		IMindComponentInterface::Execute_OnInit(behaviorObj, this);
		IMindComponentInterface::Execute_OnInit(imagineObj, this);
	}
}

void UMind::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (Stop)
		return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Execute_DoWish(this);
	Execute_DoBehavior(this);
	Execute_DoImagine(this);
}

void UMind::UpdateComponent_Implementation(EMindComponentType type, TSubclassOf<UObject> newComponent)
{
	switch (type)
	{
	case EMindComponentType::WISH_COMP:
		if (WishClass != newComponent && newComponent->GetClass()->IsChildOf(UWish::StaticClass()))
		{
			if (Wish.GetObject() != nullptr)
				Wish.SetObject(nullptr);
			
			auto targetObj = NewObject<UObject>(this, newComponent->GetClass());
			Wish.SetObject(targetObj);
			Wish.SetInterface(dynamic_cast<IWishInterface*>(targetObj));
		}
		break;

	case EMindComponentType::BEHAVIOR_COMP:
		if (BehaviorClass != newComponent && newComponent->GetClass()->IsChildOf(UBehavior::StaticClass()))
		{
			if (Behavior.GetObject() != nullptr)
				Behavior.SetObject(nullptr);

			auto targetObj = NewObject<UObject>(this, newComponent->GetClass());
			Behavior.SetObject(targetObj);
			Behavior.SetInterface(dynamic_cast<IBehaviorInterface*>(targetObj));
		}
		break;

	case EMindComponentType::REMEMBER_COMP:
		if (RememberClass != newComponent && newComponent->GetClass()->IsChildOf(URemenber::StaticClass()))
		{
			if (Remember.GetObject() != nullptr)
				Remember.SetObject(nullptr);

			auto targetObj = NewObject<UObject>(this, newComponent->GetClass());
			Remember.SetObject(targetObj);
			Remember.SetInterface(dynamic_cast<IRememberInterface*>(targetObj));
		}
		break;

	case EMindComponentType::IMAGINE_COMP:
		if (ImagineClass != newComponent && newComponent->GetClass()->IsChildOf(UImagine::StaticClass()))
		{
			if (Imagine.GetObject() != nullptr)
				Imagine.SetObject(nullptr);

			auto targetObj = NewObject<UObject>(this, newComponent->GetClass());
			Imagine.SetObject(targetObj);
			Imagine.SetInterface(dynamic_cast<IImagineInterface*>(targetObj));
		}
		break;

	default:
		break;
	}
}

void UMind::DoWish_Implementation()
{
	if (IsMemberValid(Wish))
	{
		IWishInterface::Execute_UpdateWish(Wish.GetObject());
	}
}

void UMind::DoBehavior_Implementation()
{
	if (IsMemberValid(Behavior))
	{
		IBehaviorInterface::Execute_Behave(Behavior.GetObject());
	}
}

void UMind::DoImagine_Implementation()
{
	if (IsMemberValid(Imagine))
	{
		IImagineInterface::Execute_Imaging(Imagine.GetObject());
	}
}
