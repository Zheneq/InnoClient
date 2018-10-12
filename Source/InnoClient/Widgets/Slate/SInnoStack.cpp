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
}

void SInnoStack::Update(const TArray<TSharedPtr<SInnoCard>>& Cards, int32 SplayStart, int32 SplayEnd)
{
	if (VBox.IsValid())
	{
		UpdateCards_Internal(Cards);
		UpdateSplay_Internal(Cards, SplayStart, SplayEnd);
	}
}

void SInnoStack::UpdateCards_Internal(const TArray<TSharedPtr<SInnoCard>>& Cards)
{
	VBox->ClearChildren();
	State.Empty();

	if (Cards.Num() == 0)
	{
		SetVisibility(EVisibility::Collapsed);
	}
	else
	{
		SetVisibility(bInteractive ? EVisibility::SelfHitTestInvisible : EVisibility::HitTestInvisible);

		for (int32 i = 0; i < Cards.Num(); ++i)
		{
			const TSharedPtr<SInnoCard> CardWidget = Cards[i];

			State.Add(CardWidget->GetCardId());

			CardWidget->SetRespectMinimalHeight(false);
			CardWidget->SetHideText(bHideText);
			CardWidget->SetHideHeader(false);
			if (bInteractive)
			{
				// Keep current binding if we are borrowing the widget
				CardWidget->OnClicked.Unbind();
			}
			if (i == 0)
			{
				if (bLocalPlayer && bInteractive)
				{
					CardWidget->OnClicked.BindRaw(this, &SInnoStack::TopCardClicked);
				}
			}
			else
			{
				CardWidget->SetSelectiveHideText(true);

				if (!bLocalPlayer)
				{
					CardWidget->SetHideHeader(true);
				}
			}

			VBox->AddSlot()
				.Padding(FMargin(0, 2))
				[
					CardWidget.ToSharedRef()
				];
		}
	}
}

void SInnoStack::UpdateSplay_Internal(const TArray<TSharedPtr<SInnoCard>>& Cards, int32 SplayStart, int32 SplayEnd)
{
	MySplayStart = SplayStart;
	MySplayEnd = SplayEnd;

	for (int32 i = 0; i < Cards.Num(); ++i)
	{
		const TSharedPtr<SInnoCard> CardWidget = Cards[i];
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
