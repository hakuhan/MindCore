#include "Basic/Wish.h"

void UWish::Init(const FWishInfo &Info)
{
	m_Info = Info;

	Reset();
}

void UWish::UpdateWish_Implementation()
{
	if (m_Data.SeekingWishes.Num() == 0)
	{
		return;
	}
	
	for (int i = m_Data.SeekingWishes.Num() - 1; i >= 0; --i)
	{
		bool wishOver = false;
		int index = m_Data.OwnedThings.IndexOfByPredicate([&](const FThing &target) { return target.Name == m_Data.SeekingWishes[i].Name; });
		if (index >= 0)
		{
			if (!m_Data.OwnedThings[index].NeverEnding && m_Data.OwnedThings[index].Number >= m_Data.SeekingWishes[i].Number)
			{
				wishOver = true;
			}
			else if (m_Data.OwnedThings[index].NeverEnding && m_Data.OwnedThings[index].Disappearance)
			{
				wishOver = true;
			}
		}
		else if (m_Data.SeekingWishes[i].Disappearance)
		{
			wishOver = true;
		}

		if (wishOver)
		{
			if (index < 0)
			{
				m_Data.OwnedThings.Add(m_Data.SeekingWishes[i]);
			}
			m_Data.SeekingWishes.RemoveAt(i);
		}
	}
}

bool UWish::CreateWish_Implementation(const FThing &wish)
{
	int index = m_Info.Wishes.IndexOfByPredicate([&](const FThing &target) { return wish.Name == target.Name; });
	if (index < 0)
	{
		m_Info.Wishes.Add(wish);
	}
	else
	{
		m_Info.Wishes[index].Number += wish.Number;
	}

	return true;
}

bool UWish::ObtainThing_Implementation(const FThing &thing)
{
	if (thing.NeverEnding)
	{
		return false;
	}

	int index = m_Data.OwnedThings.IndexOfByPredicate([&](const FThing &target) { return target == thing; });
	if (index < 0)
	{
		m_Data.OwnedThings.Add(thing);
	}
	else 
	{
		m_Data.OwnedThings[index].Number += thing.Number;
	}

	return true;
}

bool UWish::LoseWish_Implementation(const FThing &wish)
{
	return m_Data.SeekingWishes.RemoveAll([&](const FThing &target) { return target == wish; }) > 0;
}

bool UWish::LoseThing_Implementation(const FThing &thing)
{
	int index = m_Data.OwnedThings.IndexOfByPredicate([&](const FThing &target) { return target == thing; });
	if (index >= 0)
	{
		if (thing.Number < m_Data.OwnedThings[index].Number)
		{
			m_Data.OwnedThings[index].Number -= thing.Number;
		}
		else
		{
			m_Data.OwnedThings.RemoveAt(index);
		}

		return true;
	}

	return false;
}

bool UWish::GetWishes_Implementation(TArray<FThing>& wishes)
{
	for (int i = 0; i < m_Data.SeekingWishes.Num(); ++i)
	{
		wishes.Add(m_Data.SeekingWishes[i]);
	}

	return m_Data.SeekingWishes.Num() > 0;
}

bool UWish::CheckThingOwned_Implementation(const FThing& thing)
{
	int thingIndex = m_Data.OwnedThings.Find(thing);
	return thingIndex >= 0 && thing.Number <= m_Data.OwnedThings[thingIndex].Number;
}

void UWish::Reset()
{
	m_Data.SeekingWishes = m_Info.Wishes;
	m_Data.OwnedThings.Empty();
}

void UWish::UnInstall_Implementation()
{
	Mind = nullptr;
	m_Info.Empty();
	m_Data.Empty();
    UE_LOG(LogTemp, Warning, TEXT("Wish uninstall!"))
}

bool UWish::UpdatePriority_Implementation(const FThing& target, int priority)
{
	bool result = false;
	for (int i = 0; i < m_Data.SeekingWishes.Num(); ++i)
	{
		if (m_Data.SeekingWishes[i] == target)
		{
			m_Data.SeekingWishes[i].Priority = priority;
			result = true;
			OnUpdateThing.ExecuteIfBound(m_Data.SeekingWishes[i]);
		}
	}

	return result;
}

void UWish::OnInit_Implementation(UMind *mind)
{
	Mind = mind;

	Reset();
}