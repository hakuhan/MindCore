#include "Basic/Remember.h"

void URemenber::OnInit_Implementation(UMind *mind)
{
    Mind = mind;
}

bool URemenber::Remember_Implementation(const FMemoryFragment& target)
{
    if (FindIndex(target.Name) < 0)
    {
        m_Memories.Add(target);
        return true;
    }
    return false;
}

bool URemenber::Remind_Implementation(const FString& thingID, FMemoryFragment &outfragment)
{
    bool result = false;
    int targetIndex = FindIndex(thingID);

    if (targetIndex > -1)
    {
        result = true;
        outfragment = m_Memories[targetIndex];
    }
    else
    {
        outfragment = FMemoryFragment();
        outfragment.Name = thingID;
        outfragment.Type = EMemoryType::Memory_Empty;
    }

    return result;
}

bool URemenber::Forget_Implementation(const FString& thingID)
{
    bool result = false;
    int targetIndex = FindIndex(thingID);

    if (targetIndex >= 0)
    {
        m_Memories.RemoveAt(targetIndex);
        result = true;
    }

    return result;
}

bool URemenber::Share_Implementation(const FString &memoryName, const TScriptInterface<IRememberInterface> &target)
{
    bool result = false;
    int targetIndex = FindIndex(memoryName);
    
    if (targetIndex >= 0 && target.GetObject() != nullptr)
    {
        // Get memory
        FMemoryFragment memoryFragment = m_Memories[targetIndex];

        // Send
        target->Accept(memoryName, memoryFragment);
        result = true;
    }

    return result;
}

void URemenber::Accept_Implementation(const FString &thingID, const FMemoryFragment &inMemory)
{
    m_Memories.Add(inMemory);
}

void URemenber::UnInstall_Implementation()
{
    m_Memories.Empty();
    Mind = nullptr;
    UE_LOG(LogTemp, Warning, TEXT("Remember uninstall!"))
}

void URemenber::Clean()
{
    m_Memories.Empty();
}

int URemenber::FindIndex(const FString& memoryName)
{
    return m_Memories.IndexOfByPredicate([&](const FMemoryFragment& memory){
        return memory.Name == memoryName;
    });
}
