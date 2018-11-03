// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoLog.h"

void UInnoLog::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyWidget.Reset();
}

TSharedRef<SWidget> UInnoLog::RebuildWidget()
{
	MyWidget = SNew(SInnoGameLog);

	return MyWidget.ToSharedRef();
}

const FText UInnoLog::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "Innovation", "Innovation");
}

void UInnoLog::AddMessage(const FText& Message, bool bReplaceLastMessage)
{
	if (MyWidget.IsValid())
	{
		MyWidget->AddMessage(Message, bReplaceLastMessage);
	}
}

void UInnoLog::Clear()
{
	if (MyWidget.IsValid())
	{
		MyWidget->Clear();
	}
}
