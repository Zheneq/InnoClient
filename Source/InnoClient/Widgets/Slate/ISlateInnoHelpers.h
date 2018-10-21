// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Slate/SInnoCard.h"
#include "InnoCards.h"
#include "ISlateInnoHelpers.generated.h"

UINTERFACE()
class USlateInnoState : public UInterface
{
	GENERATED_BODY()
};

class ISlateInnoState
{
	GENERATED_BODY()

protected:
	// My cards
	TArray<FInnoCardInfo> State;

public:
	// Get current card list
	const TArray<FInnoCardInfo>& GetState() { return State; }

	// Check if state is equal to explicit card id array
	bool StateIsEqualTo(const TArray<int32>& Other) const
	{
		if (State.Num() != Other.Num())
		{
			return false;
		}

		for (int32 i = 0; i < State.Num(); ++i)
		{
			if (!State[i].IsExplicit() || State[i].CardId != Other[i])
			{
				return false;
			}
		}

		return true;
	}

	// Pass negative to add to the end
	void AddCardAt(TSharedRef<SInnoCard> Card, int32 Position)
	{
		const int32 Num = State.Num();
		if (Position < 0 || Position > Num)
		{
			Position = Num;
		}
		State.Insert(Card->GetCardInfo(), Position);

		PerformCardAddition(Card, Position);
	}

	//
	virtual TSharedPtr<SInnoCard> RemoveCard(FInnoCardInfo Card)
	{
		const int32 Position = State.Find(Card);
		if (Position == INDEX_NONE)
		{
			return nullptr;
		}
		State.RemoveAt(Position);

		TSharedPtr<SInnoCard> Widget = PerformCardRemoval(Position);

		if (Widget.IsValid())
		{
			Widget->UnbindAll();
		}

		return Widget;
	}

	// Rebuild state using card array.
	// Call this when you cannot insert card at requested position and add it elsewhere.
	template <class SInnoCardType>
	void RebuildState(const TArray<TSharedPtr <SInnoCardType> >& Cards)
	{
		State.Empty();
		for (const auto& Card : Cards)
		{
			State.Add(StaticCastSharedPtr<SInnoCardBase>(Card)->GetCardInfo());
		}
	}

private:
	// Implementation: Remove the card widget and return a pointer to it
	virtual TSharedPtr<class SInnoCard> PerformCardRemoval(uint32 Position) = 0;

	// Implementation: Remove the card widget and return a pointer to it
	virtual void PerformCardAddition(TSharedRef<class SInnoCard> Card, uint32 Position) = 0;

};
