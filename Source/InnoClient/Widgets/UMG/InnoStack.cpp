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

FReply UInnoStack::SlateHandleClicked(FInnoCardInfo Card)
{
	OnTopCardClicked.Broadcast();

	return FReply::Handled();
}

void UInnoStack::Update(const TArray<int32>& Cards, EInnoSplay Splay, bool bWarning)
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
			// UE_LOG(LogInno, Warning, TEXT("UInnoStack::Update: Performing full update (%d)"), bWarning);

			TArray< TSharedPtr<SInnoCard> > NewWidgets;
			for (const int32 CardId : Cards)
			{
				NewWidgets.Add(GM.Get()->CardWidgetManager->GetCard(CardId));
			}
			
			// Server orders cards from bottom to top
			Algo::Reverse(NewWidgets);

			MyWidget->Update(NewWidgets, UInnoFunctionLibrary::SplayStart(Splay), UInnoFunctionLibrary::SplayEnd(Splay), bWarning);
		}
		else
		{
			// UE_LOG(LogInno, Warning, TEXT("UInnoStack::Update: Performing partial update (%d)"), bWarning);

			MyWidget->Update(UInnoFunctionLibrary::SplayStart(Splay), UInnoFunctionLibrary::SplayEnd(Splay), bWarning);
		}
	}
#if !UE_BUILD_SHIPPING
	else
	{
		UE_LOG(LogInno, Warning, TEXT("UInnoStack::Update: Failed (MyWidget: %d, GM: %d)"), MyWidget.IsValid(), GM.IsValid());
	}
#endif // !UE_BUILD_SHIPPING
}

void UInnoStack::UpdateSplayAndWarning(EInnoSplay Splay, bool bWarning)
{
	MyWidget->Update(UInnoFunctionLibrary::SplayStart(Splay), UInnoFunctionLibrary::SplayEnd(Splay), bWarning);
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
