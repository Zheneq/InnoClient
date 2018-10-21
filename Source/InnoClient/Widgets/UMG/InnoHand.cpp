// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoHand.h"
#include "GMInno.h"
#include "CardWidgetManager.h"
#include "InnoClient.h"
#include "Engine.h"

void UInnoHand::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyWidget.Reset();
}

TSharedRef<SWidget> UInnoHand::RebuildWidget()
{
	MyWidget = SNew(SInnoHand)
		.Orientation(Orientation)
		.OnCardClicked(BIND_UOBJECT_DELEGATE(SInnoCard::FOnCardClicked, SlateHandleClicked));

	return MyWidget.ToSharedRef();
}

const FText UInnoHand::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "Innovation", "Innovation");
}

FReply UInnoHand::SlateHandleClicked(FInnoCardInfo Card)
{
	OnCardClicked.Broadcast(Card);

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
		if (!MyWidget->StateIsEqualTo(Cards))
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
		UE_LOG(LogInno, Warning, TEXT("UInnoHand::Update: Failed (MyWidget: %d, GM: %d)"), MyWidget.IsValid(), GM.IsValid());
	}
#endif // UE_BUILD_DEBUG
}

void UInnoHand::SetChildrenEnabled(bool bNewChildrenEnabled)
{
	if (MyWidget.IsValid())
	{
		MyWidget->SetChildrenEnabled(bNewChildrenEnabled);
	}
}
