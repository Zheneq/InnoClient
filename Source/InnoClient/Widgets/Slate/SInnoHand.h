#pragma once

#include "SlateBasics.h"
#include "InnoCards.h"
#include "Widgets/Slate/SInnoCard.h"

class INNOCLIENT_API SInnoHand : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SInnoHand) :
		_Orientation(EOrientation::Orient_Vertical)
	{}
	SLATE_ARGUMENT(EOrientation, Orientation)
	SLATE_EVENT(SInnoCard::FOnCardClicked, OnTopCardClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	TSharedPtr<class SInnoCardViewPanel> Panel;

	TArray<TSharedPtr<SInnoCard>> MyCards;

public:
	// Update contents
	void Update(TArray<TSharedPtr<SInnoCard>> Cards);

	FReply CardClicked(int32 CardId);

	SInnoCard::FOnCardClicked OnCardClicked;
};
