// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SInnoCard.h"
#include "SMyBorder.h"
#include "SlateExtras.h"
#include "Widgets/Layout/SScaleBox.h"
#include "InnoCards.h"
#include "GIInno.h"
#include "InnoClient.h"
#include "Engine.h"

void SInnoCardIcon::Construct(const FArguments& InArgs)
{
	if (!InArgs._Style || !InArgs._Style->Icons.Contains(InArgs._Icon))
	{
		UE_LOG(LogInno, Warning, TEXT("SInnoCardIcon: No style specified. Icon will not be created."));
		return;
	}

	UTexture2D* Image = InArgs._Style->Icons[InArgs._Icon];

	IconBrush.SetResourceObject(Image);
	IconBrush.ImageSize.X = Image->GetSurfaceWidth();
	IconBrush.ImageSize.Y = Image->GetSurfaceHeight();
	IconBrush.DrawAs = ESlateBrushDrawType::Image;

	Icon = InArgs._Icon;

	ChildSlot
		[
			SNew(SBox)
			.WidthOverride(64)
			.HeightOverride(64)
			[
				SAssignNew(Img, SImage)
				.Image(&IconBrush)
			]
			
		];
}

// BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION // disables compiler optimization (optimizing complex widgets is a chore for the compiler)
void SInnoCard::Construct(const FArguments& InArgs)
{
	SInnoCardBase::Construct(SInnoCardBase::FArguments()
		.Style(InArgs._Style)
		.OnClicked(InArgs._OnClicked)
		.OnHovered(InArgs._OnHovered)
		.OnUnhovered(InArgs._OnUnhovered)
	);


	UpdateContent(InArgs._Card);
}

TSharedPtr<SWidget> SInnoCard::ConstructContents()
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			// HEADER
			SAssignNew(SBHeader, SBox)
			.HeightOverride(Style->HeaderHeight)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(Style->IconsFillWidth)
				[
					SNew(SScaleBox)
					.Stretch(EStretch::ScaleToFit)
					[
						SAssignNew(GPIcons, SGridPanel)
					]
				]
				+ SHorizontalBox::Slot()
				.FillWidth(Style->NameFillWidth)
				.VAlign(Style->NameVAlign)
				.HAlign(Style->NameHAlign)
				[
					SAssignNew(TxtCardName, STextBlock)
					.TextStyle(&Style->NameTextStyle)
					.Margin(Style->NamePadding)
					.Justification(Style->NameJustification)
					.AutoWrapText(true)
				]
				+ SHorizontalBox::Slot()
				.FillWidth(Style->AgeFillWidth)
				[
					SAssignNew(BAge, SBorder)
					.ShowEffectWhenDisabled(false)
					.VAlign(EVerticalAlignment::VAlign_Center)
					[
						SAssignNew(TxtAge, STextBlock)
						.TextStyle(&Style->AgeTextStyle)
						.Margin(Style->AgePadding)
					]
				]
			]
		]
		// Card text
		+ SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(EVerticalAlignment::VAlign_Fill)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SAssignNew(VBIconEffects, SVerticalBox)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(EVerticalAlignment::VAlign_Fill)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SAssignNew(VBEffects, SVerticalBox)
		];
}

void SInnoCard::UpdateContent(const FInnoCard& Card)
{
	check(Style && BBackground.IsValid() && BAge.IsValid() && TxtCardName.IsValid() && TxtAge.IsValid());

	CardInfo = FInnoCardInfo::FromCard(Card);

	TxtCardName->SetText(Card.Name);
	BBackground->SetBorderImage(Style->BackgroundColorBrush(Card.Color));

	if (Card.Age <= 0)
	{
		// Special achievement
		BAge->SetVisibility(EVisibility::Collapsed);
		GPIcons->SetVisibility(EVisibility::Collapsed);
	}
	else
	{
		// Common card
		GPIcons->SetVisibility(EVisibility::HitTestInvisible);
		BAge->SetVisibility(EVisibility::HitTestInvisible);
		BAge->SetBorderImage(Style->AgeBackgroundColorBrush(Card.Set));
		TxtAge->SetText(FText::FromString(FString::FromInt(Card.Age)));
	}

	// Filling icons
	const int32 X[] = { 0, 0, 1, 2, 2, 1 };
	const int32 Y[] = { 0, 1, 1, 1, 0, 0 };

	check(GPIcons.IsValid());
	GPIcons->ClearChildren();
	ICIIcons.Empty();
	for (int32 i = 0; i < Card.Icons.Num(); ++i)
	{
		TSharedPtr<SInnoCardIcon> Icon;
		GPIcons->AddSlot(X[i], Y[i])
			[
				SAssignNew(Icon, SInnoCardIcon)
				.Icon(Card.Icons[i])
				.Style(Style)
			];
		ICIIcons.Add(Icon);
	}

	// Filling dogmas
	check(VBIconEffects.IsValid());
	VBIconEffects->ClearChildren();
	for (const auto& Dogma : Card.Echoes)
	{
		VBIconEffects->AddSlot()
			.AutoHeight()
			.VAlign(EVerticalAlignment::VAlign_Fill)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			[
				SNew(SRichTextBlock)
				.Text(Dogma)
				.WrapTextAt(Style->DogmaWrapTextAt)
				.TextStyle(&Style->EchoTextStyle)
				.Margin(Style->EchoInspirePadding)
				.LineHeightPercentage(Style->EchoLineHeightPercentage)
				.Justification(Style->EchoJustification)
				.DecoratorStyleSet(&FInnoWidgetStyles::Get())
				+ SRichTextBlock::ImageDecorator()
			];
	}
	for (const auto& Dogma : Card.Inspire)
	{
		VBIconEffects->AddSlot()
			.AutoHeight()
			.VAlign(EVerticalAlignment::VAlign_Fill)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			[
				SNew(SRichTextBlock)
				.Text(Dogma)
				.WrapTextAt(Style->DogmaWrapTextAt)
				.TextStyle(&Style->InspireTextStyle)
				.Margin(Style->EchoInspirePadding)
				.LineHeightPercentage(Style->InspireLineHeightPercentage)
				.Justification(Style->InspireJustification)
				.DecoratorStyleSet(&FInnoWidgetStyles::Get())
				+ SRichTextBlock::ImageDecorator()
			];
	}

	check(VBEffects.IsValid());
	VBEffects->ClearChildren();
	VBEffects->AddSlot()
		.AutoHeight()
		.VAlign(EVerticalAlignment::VAlign_Fill)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SNew(SSeparator)
			.Orientation(EOrientation::Orient_Horizontal)
			.Thickness(Style->BorderThickness)
			.ColorAndOpacity(Style->ButtonStyle.Normal.TintColor)
		];
	for (const auto& Dogma : Card.Dogmas)
	{
		VBEffects->AddSlot()
			.AutoHeight()
			.VAlign(EVerticalAlignment::VAlign_Fill)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			[
				SNew(SRichTextBlock)
				.Text(Dogma)
				.WrapTextAt(Style->DogmaWrapTextAt)
				.TextStyle(&Style->DogmaTextStyle)
				.Margin(Style->DogmaKarmaPadding)
				.LineHeightPercentage(Style->DogmaLineHeightPercentage)
				.Justification(Style->DogmaJustification)
				.DecoratorStyleSet(&FInnoWidgetStyles::Get())
				+ SRichTextBlock::ImageDecorator()
			];
	}

	if (!Card.Karma.IsEmpty())
	{
		VBEffects->AddSlot()
			.AutoHeight()
			.VAlign(EVerticalAlignment::VAlign_Fill)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			[
				SNew(SRichTextBlock)
				.Text(Card.Karma)
				.WrapTextAt(Style->DogmaWrapTextAt)
				.TextStyle(&Style->KarmaTextStyle)
				.Margin(Style->DogmaKarmaPadding)
				.LineHeightPercentage(Style->KarmaLineHeightPercentage)
				.Justification(Style->KarmaJustification)
				.DecoratorStyleSet(&FInnoWidgetStyles::Get())
				+ SRichTextBlock::ImageDecorator()
			];
	}

	if (!Card.Text.IsEmpty())
	{
		VBEffects->AddSlot()
			.AutoHeight()
			.VAlign(EVerticalAlignment::VAlign_Fill)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			[
				SNew(SRichTextBlock)
				.Text(Card.Text)
				.WrapTextAt(Style->DogmaWrapTextAt)
				.TextStyle(&Style->KarmaTextStyle)
				.Margin(Style->DogmaKarmaPadding)
				.LineHeightPercentage(Style->KarmaLineHeightPercentage)
				.Justification(Style->KarmaJustification)
				.DecoratorStyleSet(&FInnoWidgetStyles::Get())
				+ SRichTextBlock::ImageDecorator()
			];
	}
}
// END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInnoCard::ResetAppearance()
{
	SetRespectMinimalHeight(true);
	SetHideText(false);
	SetHideHeader(false);
	SetSelectiveHideText(false);
}

void SInnoCard::SetRespectMinimalHeight(bool bNewRespectMinimalHeight)
{
	bRespectMinimalHeight = bNewRespectMinimalHeight;
	if (SizeBox.IsValid() && Style)
	{
		SizeBox->SetMinDesiredHeight(bRespectMinimalHeight ? Style->MinHeight : FOptionalSize());
	}
}

void SInnoCard::SetHideHeader(bool bNewHideHeader)
{
	bHideHeader = bNewHideHeader;
	if (SBHeader.IsValid())
	{
		SBHeader->SetVisibility(bHideHeader ? EVisibility::Collapsed : EVisibility::HitTestInvisible);
	}
}

void SInnoCard::SetHideText(bool bNewHideText)
{
	bHideText = bNewHideText;
	if (VBIconEffects.IsValid() && VBEffects.IsValid())
	{
		VBIconEffects->SetVisibility(bHideText ? EVisibility::Collapsed : EVisibility::HitTestInvisible);
		VBEffects->SetVisibility(bHideText ? EVisibility::Collapsed : EVisibility::HitTestInvisible);
	}
}

void SInnoCard::SetSelectiveHideText(bool bNewSelectiveHideText)
{
	bSelectiveHideText = bNewSelectiveHideText;
}

void SInnoCard::StackInvalidate(int32 SplayStart, int32 SplayEnd, bool bTopCard)
{

	if (!bHideText && bSelectiveHideText)
	{
		bool bIconEffectVisible = false;

		for (int32 i = 0; i < ICIIcons.Num(); ++i)
		{
			if (ICIIcons[i].IsValid())
			{
				const bool bVisible = bTopCard || (i >= SplayStart && i < SplayEnd);
				ICIIcons[i]->SetVisible(bVisible);

				bIconEffectVisible |= bVisible && (ICIIcons[i]->Icon == EInnoResource::IR_Echo || ICIIcons[i]->Icon == EInnoResource::IR_Inspire);
			}

		}

		VBIconEffects->SetVisibility(bIconEffectVisible ? EVisibility::HitTestInvisible : EVisibility::Collapsed);
		VBEffects->SetVisibility(bTopCard ? EVisibility::HitTestInvisible : EVisibility::Collapsed);
	}
}
