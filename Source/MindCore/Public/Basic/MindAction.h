#pragma once

#include "CoreMinimal.h"
#include "Core/ActionInterface.h"
#include "MindAction.generated.h"

class UMind;

UCLASS(Blueprintable)
class MINDCORE_API UMindAction : public UObject, public IActionInterface
{
	GENERATED_BODY()

public:
	UMindAction()
	{
		m_State = EActionState::Action_Inactive;
		m_bLose = false;
	}

	void Init_Implementation(AActor* owner) override
	{
		m_Owner = owner;
	}

	bool IsCost_Implementation() override
	{
		return true;
	}

	EActionState GetState_Implementation() override
	{
		return m_State;
	}


	bool CanEfficacyLose_Implementation() override
	{
		return false;
	}


	bool CheckLose_Implementation() override
	{
		return m_bLose;
	}


	FUpdateDifficultyDelegate& GetDifficultyDelegate() override
	{
		return OnUpdateDifficulty;
	}

	FSwitchDifficultyDelegate& GetSwitchDifficultyDelegate() override
	{
		return OnSwitchDifficulty;
	}

	void UpdateDifficulty_Implementation(EActionDifficulty difficulty) override
	{
		OnUpdateDifficulty.ExecuteIfBound(this, difficulty);
	}

	void SwitchDifficulty_Implementation(const FString& targetActionName) override
	{
		OnSwitchDifficulty.ExecuteIfBound(this, targetActionName);
	}

	UObject* GetImagine_Implementation(TSubclassOf<UImagine> imagineClass) override
	{
		if (m_Owner)
		{
			auto mind = dynamic_cast<UMind*>(m_Owner->GetComponentByClass(UMind::StaticClass()));
			if (mind)
			{
				return mind->Imagine.GetObject();
			}
		}

		return nullptr;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		EActionState m_State;
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		AActor* m_Owner;
	UPROPERTY(BlueprintReadwrite)
		bool m_bLose;

	FUpdateDifficultyDelegate OnUpdateDifficulty;
	FSwitchDifficultyDelegate OnSwitchDifficulty;

};