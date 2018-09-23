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
	MyCard = SNew(SInnoCard).Card(UInnoFunctionLibrary::GetCard(this, CardId));

	return MyCard.ToSharedRef();
}

const FText UInnoCardWidget::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "Innovation", "Innovation");
}
