// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Slate/SInnoCard.h"
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
	TArray<int32> State;

public:
	// Get current card list
	const TArray<int32>& GetState() { return State; }

	// Pass negative to add to the end
	void AddCardAt(TSharedRef<SInnoCard> Card, int32 Position)
	{
		const int32 Num = State.Num();
		if (Position < 0 || Position > Num)
		{
			Position = Num;
		}
		State.Insert(Card->GetCardId(), Position);

		PerformCardAddition(Card, Position);
	}

	//
	virtual TSharedPtr<SInnoCard> RemoveCard(int32 CardId)
	{
		const int32 Position = State.Find(CardId);
		if (Position == INDEX_NONE)
		{
			return nullptr;
		}
		State.RemoveAt(Position);

		return PerformCardRemoval(Position);
	}

private:
	// Implementation: Remove the card widget and return a pointer to it
	virtual TSharedPtr<SInnoCard> PerformCardRemoval(uint32 Position) = 0;

	// Implementation: Remove the card widget and return a pointer to it
	virtual void PerformCardAddition(TSharedRef<SInnoCard> Card, uint32 Position) = 0;

};
