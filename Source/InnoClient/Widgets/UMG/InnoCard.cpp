// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoCard.h"
#include "InnoFunctionLibrary.h"

void UInnoCardWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyCard.IsValid())
	{
		MyCard->UpdateContent(UInnoFunctionLibrary::GetCard(this, CardId));
	}
}

void UInnoCardWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyCard.Reset();
}

TSharedRef<SWidget> UInnoCardWidget::RebuildWidget()
{
	MyCard = SNew(SInnoCard)
		.Card(UInnoFunctionLibrary::GetCard(this, CardId))
		.OnClicked(BIND_UOBJECT_DELEGATE(SInnoCard::FOnCardClicked, SlateHandleClicked))
		.OnHovered(BIND_UOBJECT_DELEGATE(SInnoCard::FOnCardSimple, SlateHandleHovered))
		.OnUnhovered(BIND_UOBJECT_DELEGATE(SInnoCard::FOnCardSimple, SlateHandleUnhovered));

	return MyCard.ToSharedRef();
}

void UInnoCardWidget::SetCardId(int32 InCardId)
{
	CardId = InCardId;
	SynchronizeProperties();
}

FReply UInnoCardWidget::SlateHandleClicked(FInnoCardInfo Card)
{
	OnClicked.Broadcast(Card.CardId);

	return FReply::Handled();
}

void UInnoCardWidget::SlateHandleHovered(FInnoCardInfo Card)
{
	OnHovered.Broadcast(Card.CardId);
}

void UInnoCardWidget::SlateHandleUnhovered(FInnoCardInfo Card)
{
	OnUnhovered.Broadcast(Card.CardId);
}

const FText UInnoCardWidget::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "Innovation", "Innovation");
}
