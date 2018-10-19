// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SInnoCardBase.h"
#include "SMyBorder.h"
#include "Widgets/Styles/InnoWidgetStyles.h"
#include "SlateExtras.h"
#include "InnoClient.h"

void SInnoCardBase::Construct(const FArguments& InArgs)
{
	check(InArgs._Style != nullptr);
	Style = InArgs._Style;

	bCanTick = false;
	CardId = 0;

	ChildSlot
		[
			ConstructCard().ToSharedRef()
		];

	OnClicked   = InArgs._OnClicked;
	OnHovered   = InArgs._OnHovered;
	OnUnhovered = InArgs._OnUnhovered;
}

// BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION // disables compiler optimization (optimizing complex widgets is a chore for the compiler)
TSharedPtr<SWidget> SInnoCardBase::ConstructCard()
{
	return SAssignNew(SizeBox, SBox)
			.WidthOverride(Style->Width)
			.MinDesiredHeight(Style->MinHeight)
			.VAlign(EVerticalAlignment::VAlign_Fill)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			[
				SNew(SButton)
				.ButtonStyle(&Style->ButtonStyle)
				.OnClicked(this, &SInnoCardBase::OnClickedHandle)
				.OnHovered(this, &SInnoCardBase::OnHoveredHandle)
				.OnUnhovered(this, &SInnoCardBase::OnUnhoveredHandle)
				.ClickMethod(EButtonClickMethod::PreciseClick)
				.TouchMethod(EButtonTouchMethod::PreciseTap)
				.ContentPadding(FMargin(0))
				.VAlign(EVerticalAlignment::VAlign_Fill)
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				[
					SAssignNew(MBBorder, SMyBorder)
					.Padding(FMargin(Style->BorderThickness))
					.ShowEffectWhenDisabled(false)
					.BorderBackgroundColor(FLinearColor::Transparent)
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
							ConstructContents().ToSharedRef()
						]
					]
				]
			];
}
// END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedPtr<SWidget> SInnoCardBase::ConstructContents()
{
	return SNullWidget::NullWidget;
}

FReply SInnoCardBase::OnClickedHandle()
{
	return OnClicked.IsBound() ? OnClicked.Execute(CardId) : FReply::Handled();
}

void SInnoCardBase::OnHoveredHandle()
{
	if (OnHovered.IsBound())
	{
		OnHovered.Execute(CardId);
	}
}

void SInnoCardBase::OnUnhoveredHandle()
{
	if (OnUnhovered.IsBound())
	{
		OnUnhovered.Execute(CardId);
	}
}
