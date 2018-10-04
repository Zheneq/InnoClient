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
		];

	bInteractable = InArgs._bInteractable;
}

void SInnoCardList::Update(TArray<TSharedPtr<SInnoCard>> Cards)
{
	if (WBox.IsValid() && Cards != MyCards)
	{
		WBox->ClearChildren();
		MyCards = Cards;

		for (TSharedPtr<SInnoCard>& CardWidget : MyCards)
		{
			CardWidget->SetRespectMinimalHeight(true);
			CardWidget->SetHideText(false);
			CardWidget->SetHideHeader(false);
			CardWidget->SetSelectiveHideText(false);
			if (bInteractable)
			{
				CardWidget->OnClicked.BindRaw(this, &SInnoCardList::CardClicked);
			}
			else
			{
				CardWidget->SetEnabled(false);
			}

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
