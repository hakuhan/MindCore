#include "Basic/Behavior.h"
#include "Basic/Wish.h"

// TODO implement
void UBehavior::Behave_Implementation()
{
    CreateBehavior();

    UpdateBehavior();

    ExecuteBehavior();
}

EBehaviorState UBehavior::GetState_Implementation()
{
    return EBehaviorState::BEHAVIOR_READY;
}

void UBehavior::OnInit_Implementation(UMind* mind)
{
    Mind = mind;
    UWish* _wish = dynamic_cast<UWish*>(Mind->Wish.GetObject());
    if (_wish)
    {
        _wish->OnUpdateThing.BindUObject(this, &UBehavior::OnUpdateThing);
    }
}

void UBehavior::UnInstall_Implementation()
{
	Mind = nullptr;
    Behaviors.Empty();
    UE_LOG(LogTemp, Warning, TEXT("Behavior uninstall!"))
}

void UBehavior::CreateBehavior()
{
    // Get wishes
    TArray<FThing> Wishes;
    IWishInterface::Execute_GetWishes(Mind->Wish.GetObject(), Wishes);

    // Find Executor to execute
    for (int i = 0; i < Wishes.Num(); ++i)
    {
        int behaviorIndex = Behaviors.IndexOfByPredicate([&](const FBehaviorItem &Item) { return Item.Target.Name == Wishes[i].Name; });
        if (behaviorIndex < 0)
        {
            // Create executor and run
            UExecutor *executor = NewObject<UExecutor>(this, ExecutorClass);
            if (executor == nullptr)
            {
                UE_LOG(LogTemp, Error, TEXT("Exector cannot be instanced!"))
            }
            else
            {
                executor->Init(Wishes[i], Mind);
                executor->OnObtainThing.BindUObject(this, &UBehavior::ObtainThing);
                executor->OnUseThing.BindUObject(this, &UBehavior::UseThing);
                IBehaviorExecutorInterface::Execute_CreateBehavior(executor);

                FBehaviorItem item;
                item.Target = Wishes[i];
                item.Executor.SetInterface(dynamic_cast<IBehaviorExecutorInterface *>(executor));
                item.Executor.SetObject(executor);
                item.State = EBehaviorState::BEHAVIOR_READY;
                Behaviors.Add(item);
            }
        }
        else
        {
            // Update executor
            IBehaviorExecutorInterface::Execute_UpdateBehavior(Behaviors[behaviorIndex].Executor.GetObject());
        }
    }
}

void UBehavior::UpdateBehavior()
{
    for (int j = 0; j < Behaviors.Num(); ++j)
    {
        auto state = IBehaviorExecutorInterface::Execute_GetExecuteState(Behaviors[j].Executor.GetObject());

        if (state == EExecutorState::EXECUTOR_FINISH)
        {
            // Update wish
            bool obtainResult = IWishInterface::Execute_ObtainThing(Mind->Wish.GetObject(), Behaviors[j].Target);
            if (obtainResult)
            {
                // Remember
                TScriptInterface<IRememberInterface> remember;
                Mind->GetRemember(remember);
                FMemoryFragment memory;
                remember->Remind(Behaviors[j].Target.Name, memory);
                memory.Target = Behaviors[j].Target;

                memory.Type = EMemoryType::Memory_Insensible;
                // TODO remember executor chain

                // TODO Clean executor datas

                remember->Remember(memory);
            }
            else
            {
                IBehaviorExecutorInterface::Execute_UpdateState(Behaviors[j].Executor.GetObject(), EExecutorState::EXECUTOR_WAITING);
            }

        }
    }
}

void UBehavior::ExecuteBehavior()
{
    TScriptInterface<IBehaviorExecutorInterface> executor;
    if (GetCurrentExecutor(executor))
    {
        // Execute
        IBehaviorExecutorInterface::Execute_ExecuteBehavior(executor.GetObject());
    }
}

void UBehavior::ObtainThing(const FThing &thing)
{
    IWishInterface::Execute_ObtainThing(Mind->Wish.GetObject(), thing);
}

void UBehavior::UseThing(const FThing &thing)
{
    IWishInterface::Execute_LoseThing(Mind->Wish.GetObject(), thing);
}

bool UBehavior::GetCurrentExecutor(TScriptInterface<IBehaviorExecutorInterface>& outExecutor)
{
    if (Behaviors.Num() <= 0)
    {
        return false;
    }

    // Find Target Behavior
    int priority = -1;
    int targetIndex = 0;
    for (int i = 0; i < Behaviors.Num(); ++i)
    {
        if (priority < Behaviors[i].Target.Priority)
        {
            auto state = IBehaviorExecutorInterface::Execute_GetExecuteState(Behaviors[i].Executor.GetObject());
            if (state == EExecutorState::EXECUTOR_READY || state == EExecutorState::EXECUTOR_EXECUTING)
            {
                targetIndex = i;
                priority = Behaviors[i].Target.Priority;
            }
        }
    }

    outExecutor = Behaviors[targetIndex].Executor;
    
    return true;
}

void UBehavior::OnUpdateThing(const FThing& thing)
{
    int thingIndex = Behaviors.IndexOfByPredicate([&](const FBehaviorItem& item){
        return item.Target == thing;
    });

    if (thingIndex >= 0)
    {
        Behaviors[thingIndex].Target = thing;
    }
}
