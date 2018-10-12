#pragma once

#include "SlateBasics.h"
#include "Widgets/Slate/SInnoCard.h"
#include "InnoCards.h"
#include "GMInno.h"
#include "Widgets/Slate/ISlateInnoHelpers.h"

class INNOCLIENT_API SInnoCardList : public SCompoundWidget, public ISlateInnoState
{
	SLATE_BEGIN_ARGS(SInnoCardList) :
		_bInteractive(false)
	{}
	// Tag is needed for log parser to figure out what this widget represents
	SLATE_ARGUMENT(FName, Tag)
	SLATE_ARGUMENT(bool, bInteractive)
	SLATE_EVENT(SInnoCard::FOnCardClicked, OnCardClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:

	// Wrap box -- container for cards
	TSharedPtr<class SWrapBox> WBox;

	// If false, will be always just a picture
	bool bInteractive : 1;

	// Tag is needed for log parser to figure out what this widget represents
	FName Tag;

public:
	// Update contents
	void Update(const TArray<TSharedPtr<SInnoCard>>& Cards);

	// Update display/interactability
	void UpdateFlags(bool bNewIsInteractive);

	FReply CardClicked(int32 CardId);

	SInnoCard::FOnCardClicked OnCardClicked;

private:
	//~ Begin ISlateInnoState Interface
	virtual TSharedPtr<SInnoCard> PerformCardRemoval(uint32 Position) override;
	virtual void PerformCardAddition(TSharedRef<SInnoCard> Card, uint32 Position) override;
	//~ End ISlateInnoState Interface
};
