// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SInnoCardList.h"
#include "CardWidgetManager.h"
#include "Widgets/Slate/SInnoCardPlaceholder.h"
#include "InnoFunctionLibrary.h"
#include "InnoClient.h"
#include "Engine.h"

void SInnoCardList::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SAssignNew(WBox, SWrapBox)
			.UseAllottedWidth(true)
		];

	OnCardClicked = InArgs._OnCardClicked;
	OnSelectionChanged = InArgs._OnSelectionChanged;
	OnSelectionValidityChanged = InArgs._OnSelectionValidityChanged;
	Tag = InArgs._Tag;

	SetVisibility(EVisibility::SelfHitTestInvisible);
}

void SInnoCardList::Update(TArray<TSharedPtr<SInnoCardBase>>& Cards, int32 NewSelectMin, int32 NewSelectMax)
{
	MyCards = Cards;
	SelectedCards.Empty();
	
	SelectMin = FMath::Max(0, NewSelectMin);
	SelectMax = FMath::Max(SelectMin, NewSelectMax);
	SetVisibility(SelectMax != 0 ? EVisibility::SelfHitTestInvisible : EVisibility::HitTestInvisible);

	if (WBox.IsValid())
	{
		WBox->ClearChildren();
		State.Empty();

		if (SelectMin == 0 && SelectMax == 1) // None is an option
		{
			WBox->AddSlot()
				.Padding(FMargin(4))
				[
					SNew(SInnoCardPlaceholder)
					.Text(NSLOCTEXT("CardListWidget", "OptionNone", "None"))
					.OnClicked(this, &SInnoCardList::CardClicked)
				];
		}

		for (int32 i = 0; i < MyCards.Num(); ++i)
		{
			TSharedPtr<SInnoCardBase> CardWidget = MyCards[i];

			CardWidget->SetIndex(i);

			State.Add(CardWidget->GetCardInfo());

			AddCard_Internal(CardWidget);
		}
	}
}

void SInnoCardList::UpdateOptions(int32 NewSelectMin, int32 NewSelectMax)
{
	Update(MyCards, NewSelectMin, NewSelectMax);
}

void SInnoCardList::AddCard_Internal(const TSharedPtr<SInnoCardBase>& CardWidget)
{
	CardWidget->ResetAppearance();
	if (SelectMax > 0)
	{
		CardWidget->OnClicked.BindRaw(this, SelectMax == 1 ? &SInnoCardList::CardClicked : &SInnoCardList::CardToggled);
	}
	//			else
	//			{
	//				CardWidget->SetEnabled(false);
	//			}

	WBox->AddSlot()
		.Padding(FMargin(4))
		[
			CardWidget.ToSharedRef()
		];
}

FReply SInnoCardList::CardClicked(FInnoCardInfo Card)
{
	return OnCardClicked.IsBound() ? OnCardClicked.Execute(Card) : FReply::Unhandled();
}

FReply SInnoCardList::CardToggled(FInnoCardInfo Card)
{
	bool bIsNowOn = false;
	if (!SelectedCards.Remove(Card.Index))
	{
		SelectedCards.Add(Card.Index);
		bIsNowOn = true;
	}

	const int32 SelectedNum = SelectedCards.Num();
	bool bWasSelectionValid = bIsSelectionValid;
	bIsSelectionValid = SelectedNum >= SelectMin && SelectedNum <= SelectMax;

	const int32 Index = State.Find(Card);
	if (Index != INDEX_NONE)
	{
		const auto WPtr = MyCards[Index];
		if (WPtr.IsValid())
		{
			WPtr->SetIsHighlighted(bIsNowOn);
		}
	}
	else
	{
		UE_LOG(LogInno, Warning, TEXT("SInnoCardList::CardToggled: I don't have card %s!"), *Card.ToString());
		SelectedCards.Remove(Card.Index);
		return FReply::Handled();
	}

	if (OnSelectionChanged.IsBound())
	{
		OnSelectionChanged.Execute(Card);
	}

	if (bWasSelectionValid != bIsSelectionValid && OnSelectionValidityChanged.IsBound())
	{
		OnSelectionValidityChanged.Execute(bIsSelectionValid);
	}

	return FReply::Handled();
}

TSharedPtr<SInnoCard> SInnoCardList::PerformCardRemoval(uint32 Position)
{
	const TSharedRef<SWidget> WidgetPtr = WBox->GetChildren()->GetChildAt(Position);
	WBox->RemoveSlot(WidgetPtr);
	MyCards.RemoveAt(Position);
	return StaticCastSharedRef<SInnoCard>(WidgetPtr);
}

void SInnoCardList::PerformCardAddition(TSharedRef<SInnoCard> CardWidget, uint32 Position)
{
	// Card list ignores position
	const bool bNeedRebuildState = MyCards.IsValidIndex(Position);
	MyCards.Add(CardWidget);
	AddCard_Internal(CardWidget);

	if (bNeedRebuildState)
	{
		RebuildState(MyCards);
	}
}
