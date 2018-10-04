// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoHand.h"
#include "GMInno.h"

void UInnoHand::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyWidget.Reset();
}

TSharedRef<SWidget> UInnoHand::RebuildWidget()
{
	MyWidget = SNew(SInnoHand);

	return MyWidget.ToSharedRef();
}

const FText UInnoHand::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "Innovation", "Innovation");
}

void UInnoHand::Update(int32 PlayerId, const struct FInnoPlayerInfo& PlayerInfo)
{

}

void UInnoHand::Play(int32 PlayId, int32 DrawDeck, const TArray<int32>& Achs, const TArray<int32>& Decree, const TArray<EInnoColor>& Inspire, const TArray<EInnoColor>& Endorse)
{

}
