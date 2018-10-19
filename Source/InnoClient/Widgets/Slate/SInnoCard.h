#pragma once

#include "SlateBasics.h"
#include "InnoCards.h"
#include "Widgets/Slate/SInnoCardBase.h"

class INNOCLIENT_API SInnoCardIcon : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SInnoCardIcon)
		: _Icon(EInnoResource::IR_Hex)
		, _Style(nullptr)
	{}
	SLATE_ARGUMENT(EInnoResource, Icon)
	SLATE_ARGUMENT(const FInnoCardStyle*, Style)
	SLATE_END_ARGS()

	void Construct(const FArguments& args);

protected:
	// Brush for Image to use
	FSlateBrush IconBrush;

	TSharedPtr<SImage> Img;

	// Is this icon visible in the stack.
	// If not, it will be grayed out.
	bool bVisible;

public:
	EInnoResource Icon;

	void SetVisible(bool bNewVisible)
	{
		bVisible = bNewVisible;

		if (Img.IsValid())
		{
			Img->SetColorAndOpacity(bVisible ? FLinearColor::White : FLinearColor(.5, .5, .5, .5));
		}
	}
};

class INNOCLIENT_API SInnoCard : public SInnoCardBase
{
public:
	SLATE_BEGIN_ARGS(SInnoCard)
		: _Style(&FInnoWidgetStyles::Get().GetWidgetStyle<FInnoCardStyle>("Style_InnoCard"))
	{}
	/** Card struct */
	SLATE_ARGUMENT(FInnoCard, Card)
	/** Style override. If not set, default one will be used. */
	SLATE_STYLE_ARGUMENT(FInnoCardStyle, Style)
	/** The delegate to execute when the card is clicked */
	SLATE_EVENT(FOnCardClicked, OnClicked)
	/** The delegate to execute when the card is hovered */
	SLATE_EVENT(FOnCardSimple, OnHovered)
	/** The delegate to execute when the card is not hovered anymore */
	SLATE_EVENT(FOnCardSimple, OnUnhovered)
	SLATE_END_ARGS()

	void Construct(const FArguments& args);

protected:
	// Construct card's inner widgets
	virtual TSharedPtr<SWidget> ConstructContents() override;

protected:
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

	// Colored age background
	TSharedPtr<SBorder> BAge;

	// Card name widget
	TSharedPtr<STextBlock> TxtCardName;

	// Card age text widget
	TSharedPtr<STextBlock> TxtAge;

	// Grid panel for card icons
	TSharedPtr<SGridPanel> GPIcons;
protected:
	bool bRespectMinimalHeight : 1;
	bool bHideHeader : 1;
	bool bHideText : 1;
	bool bSelectiveHideText : 1;

public:
	// Construct the inner contents of ther card
	void UpdateContent(const FInnoCard& Card);

	// If true, will keep minimal height even if there is little to no text.
	void SetRespectMinimalHeight(bool bNewRespectMinimalHeight);

	// Collapse card header.
	void SetHideHeader(bool bNewHideHeader);

	// Hide all text.
	void SetHideText(bool bNewHideText);

	// Hide text unless it is visible in the stack. Call StackInvalidate in order for this to take effect.
	void SetSelectiveHideText(bool bNewSelectiveHideText);

	// Call when stack has changed to grey/ungrey icons and to hide/reveal echoes and inspires.
	void StackInvalidate(int32 SplayStart, int32 SplayEnd, bool bTopCard);
};
