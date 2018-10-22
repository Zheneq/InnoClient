#pragma once

#include "SlateBasics.h"
#include "Widgets/Slate/SInnoCard.h"
#include "Widgets/Slate/ISlateInnoHelpers.h"
#include "InnoCards.h"

class INNOCLIENT_API SInnoStack : public SCompoundWidget, public ISlateInnoState
{
	SLATE_BEGIN_ARGS(SInnoStack) :
		_bLocalPlayer(false),
		_bHideText(false)
	{}
	SLATE_ARGUMENT(bool, bLocalPlayer)
	SLATE_ARGUMENT(bool, bHideText)
	SLATE_EVENT(SInnoCard::FOnCardClicked, OnTopCardClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:

	// Vertical box -- container for cards
	TSharedPtr<class SVerticalBox> VBox;

	// Card widgets we have
	TArray<TSharedPtr<SInnoCard>> MyCards;

	// Internal routine to add card in the specified position
	void AddCard_Internal(TSharedPtr<SInnoCard> Card, int32 Position);

	// Internal routine to invalidate card flags and bindings
	void InvalidateCard_Internal(int32 Position);

	// Current splay
	int32 MySplayStart, MySplayEnd;

	// Information viewed depends on whether it's the local player or an opponent
	bool bLocalPlayer : 1;

	// Option to hide all the text leaving only the header of the top card
	bool bHideText : 1;

	// Show cooperative dogma warning
	bool bWarning : 1;

public:
	// Update contents
	void Update(const TArray<TSharedPtr<SInnoCard>>& Cards, int32 SplayStart, int32 SplayEnd, bool bWarning);

	// Update but keep the cards
	void Update(int32 SplayStart, int32 SplayEnd, bool bWarning);

	// Update display/interactability
	void UpdateFlags(bool bNewIsLocalPlayer, bool bNewHideText);

	FReply TopCardClicked(FInnoCardInfo Card);

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
