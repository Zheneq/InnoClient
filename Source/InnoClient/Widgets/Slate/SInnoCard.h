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
};

class INNOCLIENT_API SInnoCard : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SInnoCard)
		: _StyleOverride(nullptr)
	{}
	SLATE_ARGUMENT(FInnoCard, Card)
	SLATE_ARGUMENT(const FInnoCardStyle*, StyleOverride)
	SLATE_END_ARGS()

	void Construct(const FArguments& args);
	void UpdateContent(const FInnoCard& Card);

protected:
	// Widget wrapper. Sometimes we want to keep minimal height of the card even if there is no text.
	TSharedPtr<SBox> SizeBox;

	// Card text, can be hidden
	TSharedPtr<SVerticalBox> VBDetails;

	// Card header, contains icons, card name and age
	TSharedPtr<SHorizontalBox> HBHeader;

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

	const FSlateBrush* BackgroundColorBrush(EInnoColor Color) const;
	const FSlateBrush* AgeBackgroundColorBrush(int32 Set) const;

public:
	FReply OnClicked() { return FReply::Unhandled(); }

	void OnHovered() {}

	void OnUnhovered() {}
};