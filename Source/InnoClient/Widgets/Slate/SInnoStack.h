#pragma once

#include "SlateBasics.h"
#include "Widgets/Slate/SInnoCard.h"
#include "InnoCards.h"
#include "GMInno.h"

class INNOCLIENT_API SInnoStack : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SInnoStack) :
		_bLocalPlayer(false),
		_bInteractable(false),
		_bHideText(false)
	{}
	SLATE_ARGUMENT(bool, bLocalPlayer)
	SLATE_ARGUMENT(bool, bInteractable)
	SLATE_ARGUMENT(bool, bHideText)
	SLATE_EVENT(SInnoCard::FOnCardClicked, OnTopCardClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:

	// Vertical box -- container for cards
	TSharedPtr<class SVerticalBox> VBox;

	// Card widgets we have
	TArray<TSharedPtr<class SInnoCard>> MyCards;

	// Current splay
	int32 MySplayStart, MySplayEnd;

	// Information viewed depends on whether it's the local player or an opponent
	bool bLocalPlayer : 1;

	// If false, will be always just a picture
	bool bInteractable : 1;

	// Option to hide all the text leaving only the header of the top card
	bool bHideText : 1;

public:
	// Update contents
	void Update(TArray<TSharedPtr<SInnoCard>> Cards, int32 SplayStart, int32 SplayEnd);

	FReply TopCardClicked(int32 CardId);

	SInnoCard::FOnCardClicked OnTopCardClicked;
};
