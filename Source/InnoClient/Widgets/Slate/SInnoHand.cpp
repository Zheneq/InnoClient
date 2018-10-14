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
	if (Panel.IsValid())
	{
		Panel->ClearChildren();
		State.Empty();

		for (TSharedPtr<SInnoCard> CardWidget : Cards)
		{
			State.Add(CardWidget->GetCardId());

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
	return StaticCastSharedRef<SInnoCard>(WidgetPtr);
}

void SInnoHand::PerformCardAddition(TSharedRef<SInnoCard> CardWidget, uint32 Position)
{
	// Inno hand ignores position, always add to the end
	Panel->AddSlot() [ CardWidget ];
}
