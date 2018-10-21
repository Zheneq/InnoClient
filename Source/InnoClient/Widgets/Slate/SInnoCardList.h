#pragma once

#include "SlateBasics.h"
#include "Widgets/Slate/SInnoCard.h"
#include "InnoCards.h"
#include "GMInno.h"
#include "Widgets/Slate/ISlateInnoHelpers.h"

class INNOCLIENT_API SInnoCardList : public SCompoundWidget, public ISlateInnoState
	// regarging ISlateInnoState
	// This class can contain both known and unknown cards.
	// And the interface in it current state does not support this.
{
public:
	DECLARE_DELEGATE_OneParam(FOnSelectionValidityChanged, bool);

	SLATE_BEGIN_ARGS(SInnoCardList)
	{}
	// Tag is needed for log parser to figure out what this widget represents
	SLATE_ARGUMENT(FName, Tag)
	SLATE_EVENT(SInnoCard::FOnCardClicked, OnCardClicked)
	SLATE_EVENT(SInnoCard::FOnCardClicked, OnSelectionChanged)
	SLATE_EVENT(FOnSelectionValidityChanged, OnSelectionValidityChanged)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:

	// Wrap box -- container for cards
	TSharedPtr<class SWrapBox> WBox;

	// Cards
	TArray< TSharedPtr<class SInnoCardBase> > MyCards;

	// Tag is needed for log parser to figure out what this widget represents
	FName Tag;

	// Minimum and maximum number of cards to be selected
	int32 SelectMin, SelectMax;

	// Indices of the cards selected
	TSet<int32> SelectedCards;

	// Does current selection match the restrictions
	bool bIsSelectionValid : 1;

public:
	// Update contents and how many options the user can select.
	// Set min and max to 0 for the widget to be non-interactive.
	void Update(TArray<TSharedPtr<SInnoCardBase>>& Cards, int32 NewSelectMin, int32 NewSelectMax);

	// Update how many options the user can select.
	// Set both to 0 for the widget to be non-interactive.
	void UpdateOptions(int32 NewSelectMin, int32 NewSelectMax);

	// Get current selection
	// Warning: Returns indices, not CardIds!
	const TSet<int32>& GetSelection() { return SelectedCards; }

	// Callback for single option
	FReply CardClicked(FInnoCardInfo CardId);

	// Callback for set option
	FReply CardToggled(FInnoCardInfo CardId);

	// EVENTS

	SInnoCard::FOnCardClicked OnCardClicked;

	// CardId: Card that was just toggled.
	SInnoCard::FOnCardClicked OnSelectionChanged;

	FOnSelectionValidityChanged OnSelectionValidityChanged;

private:
	void AddCard_Internal(const TSharedPtr<SInnoCardBase>& CardWidget);

	//~ Begin ISlateInnoState Interface
	virtual TSharedPtr<SInnoCard> PerformCardRemoval(uint32 Position) override;
	virtual void PerformCardAddition(TSharedRef<SInnoCard> Card, uint32 Position) override;
	//~ End ISlateInnoState Interface
};
