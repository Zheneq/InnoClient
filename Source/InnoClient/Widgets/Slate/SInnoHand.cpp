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
}

void SInnoHand::Update(TArray<TSharedPtr<SInnoCard>> Cards)
{
	if (Panel.IsValid() && Cards != MyCards)
	{
		Panel->ClearChildren();

		MyCards = Cards;

		for (TSharedPtr<SInnoCard> CardWidget : MyCards)
		{
			CardWidget->OnClicked.BindRaw(this, &SInnoHand::CardClicked);
			CardWidget->SetRespectMinimalHeight(true);
			CardWidget->SetHideText(false);
			CardWidget->SetSelectiveHideText(false);
			CardWidget->SetHideHeader(false);
			Panel->AddSlot() [ CardWidget.ToSharedRef() ];
		}
	}
}

FReply SInnoHand::CardClicked(int32 CardId)
{
	return OnCardClicked.IsBound() ? OnCardClicked.Execute(CardId) : FReply::Unhandled();
}
