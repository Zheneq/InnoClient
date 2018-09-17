// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoCardViewSlot.h"
#include "Components/Widget.h"

/////////////////////////////////////////////////////
// UInnoCardViewSlot

UInnoCardViewSlot::UInnoCardViewSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HorizontalAlignment = HAlign_Fill;
	VerticalAlignment = VAlign_Fill;
	Slot = NULL;
}

void UInnoCardViewSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Slot = nullptr;
}

void UInnoCardViewSlot::BuildSlot(TSharedRef<SInnoCardViewPanel> InnoCardViewPanel)
{
	Slot = &InnoCardViewPanel->AddSlot()
		.HAlign(HorizontalAlignment)
		.VAlign(VerticalAlignment)
		[
			Content == NULL ? SNullWidget::NullWidget : Content->TakeWidget()
		];
}

void UInnoCardViewSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if (Slot)
	{
		Slot->HAlign(InHorizontalAlignment);
	}
}

void UInnoCardViewSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if (Slot)
	{
		Slot->VAlign(InVerticalAlignment);
	}
}

void UInnoCardViewSlot::SynchronizeProperties()
{
	SetHorizontalAlignment(HorizontalAlignment);
	SetVerticalAlignment(VerticalAlignment);
}
