// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SInnoCardPlaceholder.h"
#include "SlateBasics.h"

// BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION // disables compiler optimization (optimizing complex widgets is a chore for the compiler)
void SInnoCardPlaceholder::Construct(const FArguments& InArgs)
{
	Text = InArgs._Text;
	CardId = -1;

	SInnoCardBase::Construct(SInnoCardBase::FArguments()
		.Style(InArgs._Style)
		.OnClicked(InArgs._OnClicked)
		.OnHovered(InArgs._OnHovered)
		.OnUnhovered(InArgs._OnUnhovered)
	);
}

TSharedPtr<SWidget> SInnoCardPlaceholder::ConstructContents()
{
	return SNew(STextBlock)
		.Text(Text)
		.TextStyle(&Style->NameTextStyle)
		.Justification(ETextJustify::Center)
		.AutoWrapText(true);

}
// END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInnoCardPlaceholder::UpdateText(FText NewText)
{
	Text = NewText;
	TxtText->SetText(Text);
}
