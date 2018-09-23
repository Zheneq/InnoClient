// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SInnoCard.h"
#include "Widgets/Styles/InnoWidgetStyles.h"
#include "Widgets/Layout/SScaleBox.h"
#include "SlateExtras.h"
#include "InnoCards.h"
#include "GIInno.h"
#include "Widgets/Other/InnoRichTextBlockImageDecorator.h"
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

	ChildSlot
		[
			SNew(SBox)
			.WidthOverride(64)
			.HeightOverride(64)
			[
				SNew(SImage)
				.Image(&IconBrush)
			]
			
		];
}

// BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION // disables compiler optimization (optimizing complex widgets is a chore for the compiler)
void SInnoCard::Construct(const FArguments& InArgs)
{
	bCanTick = false;
	DefaultStyle = &FInnoWidgetStyles::Get().GetWidgetStyle<FInnoCardStyle>("Style_InnoCard");

	SetStyle(InArgs._StyleOverride);

	check(Style);
	
	ChildSlot
		[
			SAssignNew(SizeBox, SBox)
			.WidthOverride(Style->Width)
			.MinDesiredHeight(Style->MinHeight)
			.VAlign(EVerticalAlignment::VAlign_Fill)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			[
				SNew(SButton)
				.ButtonStyle(&Style->ButtonStyle)
				.OnClicked(this, &SInnoCard::OnClicked)
				.OnHovered(this, &SInnoCard::OnHovered)
				.OnUnhovered(this, &SInnoCard::OnUnhovered)
				.ClickMethod(EButtonClickMethod::PreciseClick)
				.TouchMethod(EButtonTouchMethod::PreciseTap)
				.ContentPadding(FMargin(0))
				.VAlign(EVerticalAlignment::VAlign_Fill)
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				[
					SNew(SBorder)
					.Padding(FMargin(Style->BorderThickness))
					.BorderImage(&Style->BorderColorBrush)
					.Visibility(EVisibility::HitTestInvisible)
					.VAlign(EVerticalAlignment::VAlign_Fill)
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					[
						SAssignNew(BBackground, SBorder)
						.Padding(Style->BorderPadding)
						.ColorAndOpacity(Style->ColorAndOpacity)
						.Clipping(EWidgetClipping::ClipToBounds)
						.VAlign(EVerticalAlignment::VAlign_Fill)
						.HAlign(EHorizontalAlignment::HAlign_Fill)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								// HEADER
								SNew(SBox)
								.HeightOverride(Style->HeaderHeight)
								[
									SAssignNew(HBHeader, SHorizontalBox)
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
										.VAlign(EVerticalAlignment::VAlign_Center)
										[
											SAssignNew(TxtAge, STextBlock)
											.TextStyle(&Style->AgeTextStyle)
											.Margin(Style->AgePadding)
										]
									]
								]
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							.VAlign(EVerticalAlignment::VAlign_Fill)
							.HAlign(EHorizontalAlignment::HAlign_Fill)
							[
								SAssignNew(VBDetails, SVerticalBox)
							]
						]
					]
				]
			]
		];

	UpdateContent(InArgs._Card);
}

void SInnoCard::UpdateContent(const FInnoCard& Card)
{
	check(BBackground.IsValid() && BAge.IsValid() && TxtCardName.IsValid() && TxtAge.IsValid() && HBHeader->IsParentValid());

	TxtCardName->SetText(Card.Name);
	BBackground->SetBorderImage(TAttribute<const FSlateBrush*>::Create(TAttribute<const FSlateBrush*>::FGetter::CreateRaw(this, &SInnoCard::BackgroundColorBrush, Card.Color)));

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
		BAge->SetBorderImage(TAttribute<const FSlateBrush*>::Create(TAttribute<const FSlateBrush*>::FGetter::CreateRaw(this, &SInnoCard::AgeBackgroundColorBrush, Card.Set)));
		TxtAge->SetText(FText::FromString(FString::FromInt(Card.Age)));
	}

	// Filling icons
	const int32 X[] = { 0, 0, 1, 2, 2, 1 };
	const int32 Y[] = { 0, 1, 1, 1, 0, 0 };

	check(GPIcons.IsValid());
	GPIcons->ClearChildren();
	for (int32 i = 0; i < Card.Icons.Num(); ++i)
	{
		GPIcons->AddSlot(X[i], Y[i])
			[
				SNew(SInnoCardIcon)
				.Icon(Card.Icons[i])
				.Style(Style)
			];
	}

	// Filling dogmas
	check(VBDetails.IsValid());
	VBDetails->ClearChildren();
	for (const auto& Dogma : Card.Echoes)
	{
		VBDetails->AddSlot()
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
		VBDetails->AddSlot()
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
	VBDetails->AddSlot()
		.AutoHeight()
		.VAlign(EVerticalAlignment::VAlign_Fill)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SNew(SSeparator)
			.Orientation(EOrientation::Orient_Horizontal)
			.Thickness(Style->BorderThickness)
			.ColorAndOpacity(Style->BorderColor)
		];
	for (const auto& Dogma : Card.Dogmas)
	{
		VBDetails->AddSlot()
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
		VBDetails->AddSlot()
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
		VBDetails->AddSlot()
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

const FSlateBrush* SInnoCard::BackgroundColorBrush(EInnoColor Color) const
{
	if (Style && Style->BackgroundColorBrushes.Contains(Color))
	{
		return &Style->BackgroundColorBrushes[Color];
	}

	return nullptr;
}

const FSlateBrush* SInnoCard::AgeBackgroundColorBrush(int32 Set) const
{
	if (Style && Style->AgeBackgroundColorBrushes.IsValidIndex(Set))
	{
		return &Style->AgeBackgroundColorBrushes[Set];
	}

	return nullptr;
}
