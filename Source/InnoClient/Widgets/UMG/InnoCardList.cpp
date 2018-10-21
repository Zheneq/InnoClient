// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoCardList.h"
#include "GMInno.h"
#include "CardWidgetManager.h"
#include "InnoCards.h"
#include "InnoFunctionLibrary.h"
#include "Widgets/Slate/SInnoCardBack.h"
#include "InnoClient.h"

void UInnoCardList::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyWidget.IsValid())
	{
		MyWidget->UpdateOptions(SelectMin, SelectMax);
	}
}

void UInnoCardList::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyWidget.Reset();
}

TSharedRef<SWidget> UInnoCardList::RebuildWidget()
{
	MyWidget = SNew(SInnoCardList)
		.Tag(Tag)
		.OnCardClicked(BIND_UOBJECT_DELEGATE(SInnoCard::FOnCardClicked, SlateHandleClicked));

	return MyWidget.ToSharedRef();
}

const FText UInnoCardList::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "Innovation", "Innovation");
}

FReply UInnoCardList::SlateHandleClicked(FInnoCardInfo CardInfo)
{
	OnCardClicked.Broadcast(CardInfo.Index);

	return FReply::Handled();
}

FReply UInnoCardList::SlateHandleSelection(FInnoCardInfo CardInfo)
{
	OnCardToggled.Broadcast(CardInfo.Index);

	return FReply::Handled();
}

void UInnoCardList::SlateHandleValidity(bool bIsNowValid)
{
	OnSelectionValidityChanged.Broadcast(bIsNowValid);
}

void UInnoCardList::Update(const TArray<FInnoCardInfo>& Cards, int32 NewSelectMin, int32 NewSelectMax)
{
	SelectMin = NewSelectMin;
	SelectMax = NewSelectMax;

	if (!GM.IsValid())
	{
		const UWorld* World = GetWorld();
		GM = World ? Cast<AGMInno>(World->GetAuthGameMode()) : nullptr;
	}

	if (MyWidget.IsValid() && GM.IsValid())
	{
		if (MyWidget->GetState() != Cards)
		{
			TArray< TSharedPtr<SInnoCardBase> > NewWidgets;
			for (const FInnoCardInfo& Card : Cards)
			{
				if (Card.IsExplicit())
				{
					NewWidgets.Add(GM.Get()->CardWidgetManager->GetCard(Card.CardId));
				}
				else
				{
					NewWidgets.Add(SNew(SInnoCardBack).Card(Card));
				}
			}
			MyWidget->Update(NewWidgets, SelectMin, SelectMax);
		}
	}
#if UE_BUILD_DEBUG
	else
	{
		UE_LOG(LogInno, Warning, TEXT("UInnoCardList::Update: Failed (MyWidget: %d, GM: %d)"), MyWidget.IsValid(), GM.IsValid());
	}
#endif // UE_BUILD_DEBUG
}

void UInnoCardList::UpdateExplicit(const TArray<int32>& Cards, int32 NewSelectMin, int32 NewSelectMax)
{
	TArray<FInnoCardInfo> NewCards;
	for (int32 CardId : Cards)
	{
		NewCards.Add(FInnoCardInfo::FromCard(UInnoFunctionLibrary::GetCard(this, CardId)));
	}
	Update(NewCards, NewSelectMin, NewSelectMax);
}

void UInnoCardList::UpdateOptions(int32 NewSelectMin, int32 NewSelectMax)
{
	SelectMin = NewSelectMin;
	SelectMax = NewSelectMax;

	if (MyWidget.IsValid())
	{
		MyWidget->UpdateOptions(SelectMin, SelectMax);
	}
}

TArray<int32> UInnoCardList::GetSelectedIndices()
{
	if (MyWidget.IsValid())
	{
		return MyWidget->GetSelection().Array();
	}

	return TArray<int32>();
}
