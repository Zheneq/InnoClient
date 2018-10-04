#pragma once

#include "SlateBasics.h"
#include "Widgets/Slate/SInnoCard.h"
#include "InnoCards.h"
#include "GMInno.h"

class INNOCLIENT_API SInnoCardList : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SInnoStack) :
		_bInteractable(false)
	{}
	SLATE_ARGUMENT(bool, bInteractable)
	SLATE_EVENT(SInnoCard::FOnCardClicked, OnCardClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:

	// Wrap box -- container for cards
	TSharedPtr<class SWrapBox> WBox;

	// Card widgets we have
	TArray<TSharedPtr<class SInnoCard>> MyCards;

	// If false, will be always just a picture
	bool bInteractable : 1;

public:
	// Update contents
	void Update(TArray<TSharedPtr<SInnoCard>> Cards);

	FReply CardClicked(int32 CardId);

	SInnoCard::FOnCardClicked OnCardClicked;
};
