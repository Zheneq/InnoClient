// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoStack.h"
#include "GMInno.h"
#include "CardWidgetManager.h"
#include "InnoFunctionLibrary.h"
#include "Engine.h"
#include "Algo/Reverse.h"
#include "InnoClient.h"

void UInnoStack::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyWidget.IsValid())
	{
		MyWidget->UpdateFlags(bIsLocalPlayer, bHideText);
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
		.bLocalPlayer(bIsLocalPlayer)
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
			
			// Server orders cards from bottom to top
			Algo::Reverse(NewWidgets);

			MyWidget->Update(NewWidgets, UInnoFunctionLibrary::SplayStart(Splay), UInnoFunctionLibrary::SplayEnd(Splay));
		}
	}
#if UE_BUILD_DEBUG
	else
	{
		UE_LOG(LogInno, Warning, TEXT("UInnoStack::Update: Failed (MyWidget: %d, GM: %d)"), MyWidget.IsValid(), GM.IsValid());
	}
#endif // UE_BUILD_DEBUG
}

void UInnoStack::UpdateSplay(EInnoSplay Splay)
{
	MyWidget->UpdateSplay(UInnoFunctionLibrary::SplayStart(Splay), UInnoFunctionLibrary::SplayEnd(Splay));
}

void UInnoStack::SetIsLocalPlayer(bool bNewIsLocalPlayer)
{
	bIsLocalPlayer = bNewIsLocalPlayer;
	MyWidget->UpdateFlags(bIsLocalPlayer, bHideText);
}

void UInnoStack::SetHideText(bool bNewHideText)
{
	bHideText = bNewHideText;
	MyWidget->UpdateFlags(bIsLocalPlayer, bHideText);
}
