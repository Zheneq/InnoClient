// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SInnoStack.h"
#include "CardWidgetManager.h"
#include "InnoFunctionLibrary.h"
#include "InnoClient.h"
#include "Engine.h"

void SInnoStack::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SAssignNew(VBox, SVerticalBox)
		];

	bLocalPlayer     = InArgs._bLocalPlayer;
	bHideText        = InArgs._bHideText;
	OnTopCardClicked = InArgs._OnTopCardClicked;

	SetVisibility(EVisibility::Collapsed);
}

void SInnoStack::UpdateFlags(bool bNewIsLocalPlayer, bool bNewHideText)
{
	bLocalPlayer = bNewIsLocalPlayer;
	bHideText    = bNewHideText;

	UpdateFlags_Internal();
}

void SInnoStack::Update(const TArray<TSharedPtr<SInnoCard>>& Cards, int32 SplayStart, int32 SplayEnd, bool bNewWarning)
{
	if (VBox.IsValid())
	{
		MyCards = Cards;
		MySplayStart = SplayStart;
		MySplayEnd = SplayEnd;
		bWarning = bNewWarning;

		UpdateCards_Internal();
		UpdateSplay_Internal();
	}
}

void SInnoStack::Update(int32 SplayStart, int32 SplayEnd, bool bNewWarning)
{
	if (VBox.IsValid())
	{
		MySplayStart = SplayStart;
		MySplayEnd = SplayEnd;
		bWarning = bNewWarning;

		UpdateSplay_Internal();
		if (MyCards.Num() > 0)
		{
			MyCards[0]->SetIsHighlighted(bWarning);
		}
	}
}

void SInnoStack::UpdateCards_Internal()
{
	VBox->ClearChildren();
	State.Empty();

	if (MyCards.Num() == 0)
	{
		SetVisibility(EVisibility::Collapsed);
	}
	else
	{
		UpdateFlags_Internal();

		for (int32 i = 0; i < MyCards.Num(); ++i)
		{
			const TSharedPtr<SInnoCard> CardWidget = MyCards[i];

			State.Add(CardWidget->GetCardInfo());
			AddCard_Internal(CardWidget, i);
		}
	}
}

void SInnoStack::AddCard_Internal(TSharedPtr<SInnoCard> CardWidget, int32 Position)
{
	const bool bTopCard = !Position;

	InvalidateCard_Internal(Position);

	// (Position >= 0 ? VBox->InsertSlot(Position) : VBox->AddSlot()) // insert or append
	VBox->InsertSlot(Position)
		.Padding(FMargin(0, 2))
		.AutoHeight()
		[
			CardWidget.ToSharedRef()
		];
}

void SInnoStack::InvalidateCard_Internal(int32 Position)
{
	const bool bTopCard = !Position;
	TSharedPtr<SInnoCard> CardWidget = MyCards[Position];

	CardWidget->SetRespectMinimalHeight(false);
	CardWidget->SetHideHeader(false);
	CardWidget->SetEnabled(bTopCard);
	CardWidget->SetSelectiveHideText(!bTopCard);
	CardWidget->SetIsHighlighted(bTopCard && bWarning);
	CardWidget->OnClicked.Unbind();

	if (bTopCard)
	{
		CardWidget->OnClicked.BindRaw(this, &SInnoStack::TopCardClicked);
	}
}

void SInnoStack::UpdateFlags_Internal()
{
	if (MyCards.Num() != 0) // otherwise it's already collapsed
	{
		for (int32 i = 0; i < MyCards.Num(); ++i)
		{
			const TSharedPtr<SInnoCard> CardWidget = MyCards[i];
			
			CardWidget->SetHideText(bHideText);
			CardWidget->SetHideHeader(!bLocalPlayer && !!i);
		}
	}
}

void SInnoStack::UpdateSplay_Internal()
{
	for (int32 i = 0; i < MyCards.Num(); ++i)
	{
		const TSharedPtr<SInnoCard> CardWidget = MyCards[i];
		CardWidget->StackInvalidate(MySplayStart, MySplayEnd, !i);
	}
}

FReply SInnoStack::TopCardClicked(FInnoCardInfo Card)
{
	return OnTopCardClicked.IsBound() ? OnTopCardClicked.Execute(Card) : FReply::Unhandled();
}

TSharedPtr<SInnoCard> SInnoStack::PerformCardRemoval(uint32 Position)
{
	MyCards.RemoveAt(Position);
	if (Position == 0 && MyCards.Num() > 0)
	{
		// Invalidate new top card
		InvalidateCard_Internal(0);
	}

	const TSharedRef<SWidget> WidgetPtr = VBox->GetChildren()->GetChildAt(Position);
	VBox->RemoveSlot(WidgetPtr);
	return StaticCastSharedRef<SInnoCard>(WidgetPtr);
}

void SInnoStack::PerformCardAddition(TSharedRef<SInnoCard> CardWidget, uint32 Position)
{
	MyCards.Insert(CardWidget, Position);
	if (Position == 0 && MyCards.Num() > 1)
	{
		// Invalidate old top card
		InvalidateCard_Internal(1);
	}

	AddCard_Internal(CardWidget, Position);
}
