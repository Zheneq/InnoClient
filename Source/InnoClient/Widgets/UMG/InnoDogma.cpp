// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoDogma.h"

void UInnoDogma::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyDogma->SetText(&Text);
}

void UInnoDogma::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyDogma.Reset();
}

TSharedRef<SWidget> UInnoDogma::RebuildWidget()
{
	MyDogma = SNew(SInnoDogmaWidget);

	return MyDogma.ToSharedRef();
}

void UInnoDogma::SetText(FText InText)
{
	MyDogma->SetText(&InText);
}

//const FText UInnoDogma::GetPaletteCategory()
//{
//	return LOCTEXT("Common", "Common");
//}
