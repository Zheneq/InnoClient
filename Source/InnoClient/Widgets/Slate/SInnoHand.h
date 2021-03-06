#pragma once

#include "SlateBasics.h"
#include "InnoCards.h"
#include "Widgets/Slate/SInnoCard.h"
#include "Widgets/Slate/ISlateInnoHelpers.h"

class INNOCLIENT_API SInnoHand : public SCompoundWidget, public ISlateInnoState
{
	SLATE_BEGIN_ARGS(SInnoHand) :
		_Orientation(EOrientation::Orient_Vertical)
	{}
	SLATE_ARGUMENT(EOrientation, Orientation)
	SLATE_EVENT(SInnoCard::FOnCardClicked, OnCardClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	TSharedPtr<class SInnoCardViewPanel> Panel;

	// Cards
	TArray< TSharedPtr<class SInnoCard> > MyCards;

	void AddCard_Internal(TSharedPtr<SInnoCard> Card);

public:
	// Update contents
	void Update(TArray<TSharedPtr<SInnoCard>> Cards);

	/** Shortcut for disabling/enabling all the children. */
	void SetChildrenEnabled(bool bNewChildrenEnabled);

	/** Click event handling. */
	FReply CardClicked(FInnoCardInfo Card);
	SInnoCard::FOnCardClicked OnCardClicked;

private:
	//~ Begin ISlateInnoState Interface
	virtual TSharedPtr<SInnoCard> PerformCardRemoval(uint32 Position) override;
	virtual void PerformCardAddition(TSharedRef<SInnoCard> Card, uint32 Position) override;
	//~ End ISlateInnoState Interface
};
