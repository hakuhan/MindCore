#include "Basic/Imagine.h"
#include "Basic/Behavior.h"
#include "Basic/Executor.h"

void UImagine::Imaging_Implementation()
{
	if (Mind)
	{
		if (Mind->Wish.GetObject())
			JoinWish(Mind->Wish);

		if (Mind->Behavior.GetObject())
			JoinBehavior(Mind->Behavior);

		if (Mind->Remember.GetObject())
			JoinMemory(Mind->Remember);
	}
}

void UImagine::JoinWish_Implementation(const TScriptInterface<IWishInterface>& wish)
{

}

void UImagine::JoinBehavior(const TScriptInterface<IBehaviorInterface>& behavior)
{
	UBehavior* hehaviorCtr = dynamic_cast<UBehavior*>(behavior.GetObject());
	if (hehaviorCtr)
	{
		TScriptInterface<IBehaviorExecutorInterface> executor;
		if (hehaviorCtr->GetCurrentExecutor(executor))
		{
			UExecutor* executorCtr = dynamic_cast<UExecutor*>(executor.GetObject());
			TScriptInterface<IActionInterface> sequence;
			FActionInfo actionInfo;
			FActionData action;
			if (executorCtr
				&& executorCtr->GetCurrentAction(action)
				&& action.GetCurrentActionSequence(sequence)
				&& executorCtr->GetCurrentActionInfo(actionInfo))
			{
				JoinAction(executor, sequence, actionInfo);
			}
		}
	}
}

void UImagine::JoinAction_Implementation(const TScriptInterface<IBehaviorExecutorInterface>& executor, const TScriptInterface<IActionInterface>& acton, const FActionInfo& actionInfo)
{

}

void UImagine::JoinMemory_Implementation(const TScriptInterface<IRememberInterface>& remember)
{

}

void UImagine::UnInstall_Implementation()
{
	Mind = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("Imagine uninstall!"))
}
