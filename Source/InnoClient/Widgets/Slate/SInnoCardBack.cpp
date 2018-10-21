// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SInnoCardBack.h"
#include "SlateBasics.h"

// BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION // disables compiler optimization (optimizing complex widgets is a chore for the compiler)
void SInnoCardBack::Construct(const FArguments& InArgs)
{
	CardInfo = InArgs._Card;

	SInnoCardBase::Construct(SInnoCardBase::FArguments()
		.Style(InArgs._Style)
		.OnClicked(InArgs._OnClicked)
		.OnHovered(InArgs._OnHovered)
		.OnUnhovered(InArgs._OnUnhovered)
	);
}

TSharedPtr<SWidget> SInnoCardBack::ConstructContents()
{
	BBackground->SetBorderImage(Style->AgeBackgroundColorBrush(CardInfo.Set));

	return SNew(STextBlock)
		.Text(FText::AsNumber(CardInfo.Age))
		.TextStyle(&Style->NameTextStyle)
		.Justification(ETextJustify::Center);

}
// END_SLATE_FUNCTION_BUILD_OPTIMIZATION
