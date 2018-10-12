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
	bInteractive     = InArgs._bInteractive;
	bHideText        = InArgs._bHideText;
	OnTopCardClicked = InArgs._OnTopCardClicked;

	SetVisibility(EVisibility::Collapsed);
}

void SInnoStack::UpdateFlags(bool bNewIsLocalPlayer, bool bNewIsInteractive, bool bNewHideText)
{
	bLocalPlayer = bNewIsLocalPlayer;
	bInteractive = bNewIsInteractive;
	bHideText    = bNewHideText;

	UpdateFlags_Internal();
}

void SInnoStack::Update(const TArray<TSharedPtr<SInnoCard>>& Cards, int32 SplayStart, int32 SplayEnd)
{
	if (VBox.IsValid())
	{
		MyCards = Cards;
		MySplayStart = SplayStart;
		MySplayEnd = SplayEnd;

		UpdateCards_Internal();
		UpdateSplay_Internal();
	}
}

void SInnoStack::UpdateSplay(int32 SplayStart, int32 SplayEnd)
{
	if (VBox.IsValid())
	{
		MySplayStart = SplayStart;
		MySplayEnd = SplayEnd;

		UpdateSplay_Internal();
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

			State.Add(CardWidget->GetCardId());

			CardWidget->SetRespectMinimalHeight(false);
			CardWidget->SetHideHeader(false);
			CardWidget->SetSelectiveHideText(!!i);
			CardWidget->OnClicked.Unbind();

			if (i == 0)
			{
				CardWidget->OnClicked.BindRaw(this, &SInnoStack::TopCardClicked);
			}

			VBox->AddSlot()
				.Padding(FMargin(0, 2))
				[
					CardWidget.ToSharedRef()
				];
		}
	}
}

void SInnoStack::UpdateFlags_Internal()
{
	if (MyCards.Num() != 0) // otherwise it's already collapsed
	{
		SetVisibility(bInteractive ? EVisibility::SelfHitTestInvisible : EVisibility::HitTestInvisible);

		for (int32 i = 0; i < MyCards.Num(); ++i)
		{
			const TSharedPtr<SInnoCard> CardWidget = MyCards[i];
			
			CardWidget->SetHideText(bHideText);
			CardWidget->SetEnabled(bInteractive && !i);
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

FReply SInnoStack::TopCardClicked(int32 CardId)
{
	return OnTopCardClicked.IsBound() ? OnTopCardClicked.Execute(CardId) : FReply::Unhandled();
}

TSharedPtr<SInnoCard> SInnoStack::PerformCardRemoval(uint32 Position)
{
	const TSharedRef<SWidget> WidgetPtr = VBox->GetChildren()->GetChildAt(Position);
	VBox->RemoveSlot(WidgetPtr);
	return StaticCastSharedRef<SInnoCard>(WidgetPtr);
}

void SInnoStack::PerformCardAddition(TSharedRef<SInnoCard> CardWidget, uint32 Position)
{
	VBox->InsertSlot(Position)
		.Padding(FMargin(0, 2))
		[
			CardWidget
		];
}
