#pragma once

#include "SlateBasics.h"
#include "Widgets/Slate/SInnoCard.h"
#include "Widgets/Slate/ISlateInnoHelpers.h"
#include "InnoCards.h"

class INNOCLIENT_API SInnoStack : public SCompoundWidget, public ISlateInnoState
{
	SLATE_BEGIN_ARGS(SInnoStack) :
		_bLocalPlayer(false),
		_bInteractive(false),
		_bHideText(false)
	{}
	SLATE_ARGUMENT(bool, bLocalPlayer)
	SLATE_ARGUMENT(bool, bInteractive)
	SLATE_ARGUMENT(bool, bHideText)
	SLATE_EVENT(SInnoCard::FOnCardClicked, OnTopCardClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:

	// Vertical box -- container for cards
	TSharedPtr<class SVerticalBox> VBox;

	// Card widgets we have
	TArray<TSharedPtr<SInnoCard>> MyCards;

	// Current splay
	int32 MySplayStart, MySplayEnd;

	// Information viewed depends on whether it's the local player or an opponent
	bool bLocalPlayer : 1;

	// If false, will be always just a picture
	bool bInteractive : 1;

	// Option to hide all the text leaving only the header of the top card
	bool bHideText : 1;

public:
	// Update contents
	void Update(const TArray<TSharedPtr<SInnoCard>>& Cards, int32 SplayStart, int32 SplayEnd);

	// Update splay only
	void UpdateSplay(int32 SplayStart, int32 SplayEnd);

	// Update display/interactability
	void UpdateFlags(bool bNewIsLocalPlayer, bool bNewIsInteractive, bool bNewHideText);

	FReply TopCardClicked(int32 CardId);

	SInnoCard::FOnCardClicked OnTopCardClicked;

protected:
	void UpdateCards_Internal();
	void UpdateFlags_Internal();
	void UpdateSplay_Internal();

private:
	//~ Begin ISlateInnoState Interface
	virtual TSharedPtr<SInnoCard> PerformCardRemoval(uint32 Position) override;
	virtual void PerformCardAddition(TSharedRef<SInnoCard> Card, uint32 Position) override;
	//~ End ISlateInnoState Interface
};
