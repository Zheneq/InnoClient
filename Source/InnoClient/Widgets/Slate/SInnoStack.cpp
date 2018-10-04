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

	bLocalPlayer = InArgs._bLocalPlayer;
	bInteractable = InArgs._bInteractable;
	bHideText = InArgs._bHideText;
}

void SInnoStack::Update(TArray<TSharedPtr<SInnoCard>> Cards, int32 SplayStart, int32 SplayEnd)
{
	const bool bNeedUpdate = Cards != MyCards;
	const bool bNeedSplay  = bNeedUpdate || SplayStart != MySplayStart || SplayEnd != MySplayEnd;

	if (VBox.IsValid() && bNeedUpdate)
	{
		VBox->ClearChildren();
		MyCards = Cards;

		if (MyCards.Num() == 0)
		{
			SetVisibility(EVisibility::Collapsed);
		}
		else
		{
			SetVisibility(bInteractable ? EVisibility::SelfHitTestInvisible : EVisibility::HitTestInvisible);

			for (int32 i = 0; i < MyCards.Num(); ++i)
			{
				const TSharedPtr<SInnoCard> CardWidget = MyCards[i];

				CardWidget->SetRespectMinimalHeight(false);
				CardWidget->SetHideText(bHideText);
				CardWidget->SetHideHeader(false);
				if (bInteractable)
				{
					// Keep current binding if we are borrowing the widget
					CardWidget->OnClicked.Unbind();
				}
				if (i == 0)
				{
					if (bLocalPlayer && bInteractable)
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

	if (bNeedSplay)
	{
		MySplayStart = SplayStart;
		MySplayEnd = SplayEnd;

		for (int32 i = 0; i < MyCards.Num(); ++i)
		{
			const TSharedPtr<SInnoCard> CardWidget = MyCards[i];
			CardWidget->StackInvalidate(MySplayStart, MySplayEnd, !i);
		}
	}
}

FReply SInnoStack::TopCardClicked(int32 CardId)
{
	return OnTopCardClicked.IsBound() ? OnTopCardClicked.Execute(CardId) : FReply::Unhandled();
}
