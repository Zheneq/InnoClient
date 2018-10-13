#pragma once

#include "SlateBasics.h"
#include "InnoCards.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Styles/InnoCardWidgetStyle.h"


class INNOCLIENT_API SInnoCardIcon : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SInnoCardIcon)
		: _Icon(EInnoResource::IR_Hex)
		, _Style(nullptr)
	{}
	SLATE_ARGUMENT(EInnoResource, Icon)
	SLATE_ARGUMENT(const FInnoCardStyle*, Style)
	SLATE_END_ARGS()

	void Construct(const FArguments& args);

protected:
	FSlateBrush IconBrush;

	TSharedPtr<SImage> Img;

	bool bVisible;

public:
	EInnoResource Icon;

	void SetVisible(bool bNewVisible)
	{
		bVisible = bNewVisible;

		if (Img.IsValid())
		{
			Img->SetColorAndOpacity(bVisible ? FLinearColor::White : FLinearColor::Gray);
		}
	}
};

class INNOCLIENT_API SInnoCard : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnCardClicked, int32);
	DECLARE_DELEGATE_OneParam(FOnCardSimple, int32);

	SLATE_BEGIN_ARGS(SInnoCard)
		: _StyleOverride(nullptr)
	{}
	SLATE_ARGUMENT(FInnoCard, Card)
	SLATE_ARGUMENT(const FInnoCardStyle*, StyleOverride)
	SLATE_EVENT(FOnCardClicked, OnClicked)
	SLATE_EVENT(FOnCardSimple, OnHovered)
	SLATE_EVENT(FOnCardSimple, OnUnhovered)
	SLATE_END_ARGS()

	void Construct(const FArguments& args);
	void UpdateContent(const FInnoCard& Card);

protected:
	// Widget wrapper. Sometimes we want to keep minimal height of the card even if there is no text.
	TSharedPtr<SBox> SizeBox;

	// Card text, can be hidden
	// Echo/Inspire
	TSharedPtr<SVerticalBox> VBIconEffects;

	// Card text, can be hidden
	// Dogma/Karma/Text
	TSharedPtr<SVerticalBox> VBEffects;

	// Card header, contains icons, card name and age
	TSharedPtr<SBox> SBHeader;

	// Resource icons
	TArray<TSharedPtr<SInnoCardIcon>> ICIIcons;

	// Colored card background
	TSharedPtr<SBorder> BBackground;

	// Colored age background
	TSharedPtr<SBorder> BAge;

	// Card name widget
	TSharedPtr<STextBlock> TxtCardName;

	// Card age text widget
	TSharedPtr<STextBlock> TxtAge;

	// Grid panel for card icons
	TSharedPtr<SGridPanel> GPIcons;

	int32 CardId;

public:
	FORCEINLINE int32 GetCardId() const { return CardId; }

private:
	const struct FInnoCardStyle* DefaultStyle;

	const struct FInnoCardStyle* Style;

public:
	/**  */
	void SetStyle(const struct FInnoCardStyle* StyleOverride)
	{
		if (StyleOverride)
		{
			Style = StyleOverride;
		}
		else
		{
			Style = DefaultStyle;
		}
	}

private:
	FReply OnClickedHandle() { return OnClicked.IsBound() ? OnClicked.Execute(CardId) : FReply::Handled(); }

	void OnHoveredHandle() { if (OnHovered.IsBound()) OnHovered.Execute(CardId); }

	void OnUnhoveredHandle() { if (OnUnhovered.IsBound()) OnUnhovered.Execute(CardId); }

public:

	/** The delegate to execute when the button is clicked */
	FOnCardClicked OnClicked;

	FOnCardSimple OnHovered;

	FOnCardSimple OnUnhovered;

protected:
	bool bRespectMinimalHeight;

	bool bHideHeader;

	bool bHideText;

	bool bSelectiveHideText;


public:
	void SetRespectMinimalHeight(bool bNewRespectMinimalHeight)
	{
		bRespectMinimalHeight = bNewRespectMinimalHeight;
		if (SizeBox.IsValid() && Style)
		{
			SizeBox->SetMinDesiredHeight(bRespectMinimalHeight ? Style->MinHeight : FOptionalSize());
		}
	}

	void SetHideHeader(bool bNewHideHeader)
	{
		bHideHeader = bNewHideHeader;
		if (SBHeader.IsValid())
		{
			SBHeader->SetVisibility(bHideHeader ? EVisibility::Collapsed : EVisibility::HitTestInvisible);
		}
	}

	void SetHideText(bool bNewHideText)
	{
		bHideText = bNewHideText;
		if (VBIconEffects.IsValid() && VBEffects.IsValid())
		{
			VBIconEffects->SetVisibility(bHideText ? EVisibility::Collapsed : EVisibility::HitTestInvisible);
			VBEffects->SetVisibility(bHideText ? EVisibility::Collapsed : EVisibility::HitTestInvisible);
		}
	}

	// Hide text unless it is visible in the stack. Call StackInvalidate in order for this to take effect.
	void SetSelectiveHideText(bool bNewSelectiveHideText)
	{
		bSelectiveHideText = bNewSelectiveHideText;
	}

	// Call when stack has changed to grey/ungrey icons and to hide/reveal echoes and inspires.
	void StackInvalidate(int32 SplayStart, int32 SplayEnd, bool bTopCard)
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
};
