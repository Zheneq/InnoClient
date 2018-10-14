// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoCardList.h"
#include "GMInno.h"
#include "CardWidgetManager.h"
#include "InnoClient.h"

void UInnoCardList::SynchronizeProperties()
{
	if (MyWidget.IsValid())
	{
		MyWidget->UpdateFlags(bInteractive);
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
		.bInteractive(bInteractive)
		.Tag(Tag)
		.OnCardClicked(BIND_UOBJECT_DELEGATE(SInnoCard::FOnCardClicked, SlateHandleClicked));

	return MyWidget.ToSharedRef();
}

const FText UInnoCardList::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "Innovation", "Innovation");
}

FReply UInnoCardList::SlateHandleClicked(int32 CardId)
{
	OnCardClicked.Broadcast(CardId);

	return FReply::Handled();
}

void UInnoCardList::Update(const TArray<int32>& Cards)
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
#if UE_BUILD_DEBUG
	else
	{
		UE_LOG(LogInno, Warning, TEXT("UInnoCardList::Update: Failed (MyWidget: %d, GM: %d)"), MyWidget.IsValid(), GM.IsValid());
	}
#endif // UE_BUILD_DEBUG
}
