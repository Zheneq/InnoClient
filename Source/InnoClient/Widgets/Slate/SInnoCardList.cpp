// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SInnoCardList.h"
#include "CardWidgetManager.h"
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

	bInteractive = InArgs._bInteractive;
	OnCardClicked = InArgs._OnCardClicked;
	Tag = InArgs._Tag;

	SetVisibility(bInteractive ? EVisibility::SelfHitTestInvisible : EVisibility::HitTestInvisible);
}

void SInnoCardList::UpdateFlags(bool bNewIsInteractive)
{
	bInteractive = bNewIsInteractive;
	SetVisibility(bInteractive ? EVisibility::SelfHitTestInvisible : EVisibility::HitTestInvisible);
}

void SInnoCardList::Update(const TArray<TSharedPtr<SInnoCard>>& Cards)
{
	if (WBox.IsValid())
	{
		WBox->ClearChildren();
		State.Empty();

		for (TSharedPtr<SInnoCard> CardWidget : Cards)
		{
			State.Add(CardWidget->GetCardId());

			CardWidget->SetRespectMinimalHeight(true);
			CardWidget->SetHideText(false);
			CardWidget->SetHideHeader(false);
			CardWidget->SetSelectiveHideText(false);
			if (bInteractive)
			{
				CardWidget->OnClicked.BindRaw(this, &SInnoCardList::CardClicked);
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

	}
}

FReply SInnoCardList::CardClicked(int32 CardId)
{
	return OnCardClicked.IsBound() ? OnCardClicked.Execute(CardId) : FReply::Unhandled();
}

TSharedPtr<SInnoCard> SInnoCardList::PerformCardRemoval(uint32 Position)
{
	const TSharedRef<SWidget> WidgetPtr = WBox->GetChildren()->GetChildAt(Position);
	WBox->RemoveSlot(WidgetPtr);
	return StaticCastSharedRef<SInnoCard>(WidgetPtr);
}

void SInnoCardList::PerformCardAddition(TSharedRef<SInnoCard> CardWidget, uint32 Position)
{
	// Card list ignores position
	WBox->AddSlot()
		.Padding(FMargin(4))
		[
			CardWidget
		];
}
