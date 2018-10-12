// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoStack.h"
#include "GMInno.h"
#include "CardWidgetManager.h"
#include "InnoFunctionLibrary.h"
#include "Engine.h"

void UInnoStack::SynchronizeProperties()
{
	if (MyWidget.IsValid())
	{
		MyWidget->UpdateFlags(bLocalPlayer, bInteractive, bHideText);
	}
}

void UInnoStack::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyWidget.Reset();
}

TSharedRef<SWidget> UInnoStack::RebuildWidget()
{
	MyWidget = SNew(SInnoStack)
		.bLocalPlayer(bLocalPlayer)
		.bInteractive(bInteractive)
		.bHideText(bHideText)
		.OnTopCardClicked(BIND_UOBJECT_DELEGATE(SInnoCard::FOnCardClicked, SlateHandleClicked));

	return MyWidget.ToSharedRef();
}

const FText UInnoStack::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "Innovation", "Innovation");
}

FReply UInnoStack::SlateHandleClicked(int32 CardId)
{
	OnTopCardClicked.Broadcast();

	return FReply::Handled();
}

void UInnoStack::Update(const TArray<int32>& Cards, EInnoSplay Splay)
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
			MyWidget->Update(NewWidgets, UInnoFunctionLibrary::SplayStart(Splay), UInnoFunctionLibrary::SplayEnd(Splay));
		}
	}
}
