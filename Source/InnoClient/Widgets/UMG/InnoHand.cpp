// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoHand.h"
#include "GMInno.h"
#include "CardWidgetManager.h"
#include "Engine.h"

void UInnoHand::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyWidget.Reset();
}

TSharedRef<SWidget> UInnoHand::RebuildWidget()
{
	MyWidget = SNew(SInnoHand)
		.OnCardClicked(BIND_UOBJECT_DELEGATE(SInnoCard::FOnCardClicked, SlateHandleClicked));

	return MyWidget.ToSharedRef();
}

const FText UInnoHand::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "Innovation", "Innovation");
}

FReply UInnoHand::SlateHandleClicked(int32 CardId)
{
	OnTopCardClicked.Broadcast(CardId);

	return FReply::Handled();
}

void UInnoHand::Update(const TArray<int32>& Cards)
{
	if (!GM.IsValid())
	{
		const UWorld* World = GetWorld();
		GM = World ? Cast<AGMInno>(World->GetAuthGameMode()) : nullptr;
	}

	if (MyWidget.IsValid() && GM.IsValid())
	{
		if (MyWidget->GetState() != Cards)
		{
			TArray< TSharedPtr<SInnoCard> > NewWidgets;
			for (const int32 CardId : Cards)
			{
				NewWidgets.Add(GM.Get()->CardWidgetManager->GetCard(CardId));
			}
			MyWidget->Update(NewWidgets);
		}
	}
}
