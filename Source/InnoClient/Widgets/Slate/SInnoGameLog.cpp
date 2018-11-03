// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SInnoGameLog.h"
#include "SlateExtras.h"
#include "Widgets/Slate/SInnoCard.h"
#include "InnoFunctionLibrary.h"
#include "InnoClient.h"

// BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION // disables compiler optimization (optimizing complex widgets is a chore for the compiler)
void SInnoGameLog::Construct(const FArguments& InArgs)
{
	check(InArgs._Style != nullptr && InArgs._CardStyle != nullptr);
	Style = InArgs._Style;
	CardStyle = InArgs._CardStyle;

	TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	Baseline = FontMeasure->GetBaseline(Style->TextStyle.Font);

	// Reset
	Clear();

	bCanTick = false;

	ChildSlot
		[
			SAssignNew(SB, SScrollBox)
			.Orientation(EOrientation::Orient_Vertical)
			+ SScrollBox::Slot()
			.Padding(Style->ContentPadding)
			[
				SAssignNew(VB, SVerticalBox)
			]
		];
}
// END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FSlateWidgetRun::FWidgetRunInfo SInnoGameLog::OnCreateWidgetDecoratorWidget(const FTextRunInfo& RunInfo, const ISlateStyle* InStyle) const
{
	UE_LOG(LogInno, Log, TEXT("SInnoGameLog::OnCreateWidgetDecoratorWidget: %s - %s, meta: %d"), *RunInfo.Name, *RunInfo.Content.ToString(), RunInfo.MetaData.Num());

	FInnoCardInfo CardInfo;
	if (RunInfo.Content.IsNumeric())
	{
		CardInfo = UInnoFunctionLibrary::GetCardInfo(nullptr, FCString::Atoi(*RunInfo.Content.ToString()));
	}
	else
	{
		UE_LOG(LogInno, Warning, TEXT("SInnoGameLog::OnCreateWidgetDecoratorWidget: Failed! Card id is invalid (%s)!"), *RunInfo.Content.ToString());
		return FSlateWidgetRun::FWidgetRunInfo(SNullWidget::NullWidget, Baseline);
	}

	const FInnoCard* const Card = CardInfo.IsExplicit() ? &UInnoFunctionLibrary::GetCard(nullptr, CardInfo.CardId) : nullptr;

	TSharedRef<SWidget> Widget =
		SNew(SBorder)
		.BorderImage(Card ? CardStyle->BackgroundColorBrush(Card->Color) : CardStyle->BackgroundColorBrush(EInnoColor::IC_None))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(STextBlock)
				.TextStyle(&Style->TextStyle)
				.Text(Card ? Card->Name : FText())
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(SBorder)
				.BorderImage(CardStyle->AgeBackgroundColorBrush(CardInfo.Set))
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(STextBlock)
					.TextStyle(&Style->TextStyle)
					.Text(FText::AsNumber(CardInfo.Age))
				]
			]
		];

	if (Card)
	{
		Widget->SetToolTip(
			SNew(SToolTip)
			[
				SNew(SInnoCard)
				.Card(*Card)
				.IsEnabled(false)
				.Style(CardStyle)
			]
		);
	}

	return FSlateWidgetRun::FWidgetRunInfo(Widget, Baseline + Style->InlineCardShift);
}

FReply SInnoGameLog::OnCardNameClicked()
{
	return FReply::Handled();
}

void SInnoGameLog::AddMessage(const FText& Message, bool bReplaceLastMessage)
{
	if (VB.IsValid())
	{
		if (bReplaceLastMessage)
		{
			const auto Children = VB->GetChildren();
			VB->RemoveSlot(Children->GetChildAt(Children->Num() - 1));
		}
		else
		{
			if (MessageNum >= Style->MaxMessages)
			{
				VB->RemoveSlot(VB->GetChildren()->GetChildAt(0));
			}
			else
			{
				++MessageNum; // increment if we are not at cap yet
			}
		}

		VB->AddSlot()
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.AutoHeight()
			.Padding(Style->MessagePadding)
			[
				SNew(SRichTextBlock)
				.Text(Message)
				.WrapTextAt(Style->WrapTextAt)
				.AutoWrapText(Style->AutoWrapText)
				.WrappingPolicy(Style->WrappingPolicy)
				.TextStyle(&Style->TextStyle)
				.LineHeightPercentage(Style->LineHeightPercentage)
				.Justification(Style->Justification)
				.DecoratorStyleSet(&FInnoWidgetStyles::Get())
				+ SRichTextBlock::ImageDecorator()
				+ SRichTextBlock::WidgetDecorator(TEXT("Inno.InlineCard"), this, &SInnoGameLog::OnCreateWidgetDecoratorWidget)
			];
	}

	if (SB.IsValid() && !SB->IsHovered())
	{
		SB->ScrollToEnd();
	}
}

void SInnoGameLog::Clear()
{
	if (VB.IsValid())
	{
		MessageNum = 0;
		VB->ClearChildren();
	}
}
