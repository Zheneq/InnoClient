// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SInnoHand.h"
#include "SInnoCardViewPanel.h"
#include "InnoClient.h"
#include "Engine.h"

void SInnoHand::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SAssignNew(Panel, SInnoCardViewPanel)
			.Orientation(InArgs._Orientation)
		];

	OnCardClicked = InArgs._OnCardClicked;
}

void SInnoHand::Update(TArray<TSharedPtr<SInnoCard>> Cards)
{
	MyCards = Cards;

	if (Panel.IsValid())
	{
		Panel->ClearChildren();
		State.Empty();

		for (TSharedPtr<SInnoCard> CardWidget : MyCards)
		{
			State.Add(CardWidget->GetCardInfo());
			AddCard_Internal(CardWidget);
		}
	}
}

void SInnoHand::AddCard_Internal(TSharedPtr<SInnoCard> CardWidget)
{
	CardWidget->OnClicked.BindRaw(this, &SInnoHand::CardClicked);
	CardWidget->SetRespectMinimalHeight(true);
	CardWidget->SetHideText(false);
	CardWidget->SetSelectiveHideText(false);
	CardWidget->SetHideHeader(false);

	Panel->AddSlot()
		[
			CardWidget.ToSharedRef()
		];
}

FReply SInnoHand::CardClicked(FInnoCardInfo Card)
{
	return OnCardClicked.IsBound() ? OnCardClicked.Execute(Card) : FReply::Unhandled();
}

void SInnoHand::SetChildrenEnabled(bool bNewChildrenEnabled)
{
	if (Panel.IsValid())
	{
		Panel->SetChildrenEnabled(bNewChildrenEnabled);
	}
}

TSharedPtr<SInnoCard> SInnoHand::PerformCardRemoval(uint32 Position)
{
	const TSharedRef<SWidget> WidgetPtr = Panel->GetChildren()->GetChildAt(Position);
	Panel->RemoveSlot(WidgetPtr);
	MyCards.RemoveAt(Position);
	return StaticCastSharedRef<SInnoCard>(WidgetPtr);
}

void SInnoHand::PerformCardAddition(TSharedRef<SInnoCard> CardWidget, uint32 Position)
{
	// Inno hand ignores position, always add to the end
	const bool bNeedRebuildState = MyCards.IsValidIndex(Position);
	MyCards.Add(CardWidget);
	AddCard_Internal(CardWidget);

	if (bNeedRebuildState)
	{
		RebuildState(MyCards);
	}
}
