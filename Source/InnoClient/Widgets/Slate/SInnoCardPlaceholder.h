#pragma once

#include "SlateBasics.h"
#include "Widgets/Slate/SInnoCardBase.h"

class INNOCLIENT_API SInnoCardPlaceholder : public SInnoCardBase
{
public:
	SLATE_BEGIN_ARGS(SInnoCardPlaceholder)
		: _Style(&FInnoWidgetStyles::Get().GetWidgetStyle<FInnoCardStyle>("Style_InnoCard"))
	{}
	/** Text to show on the placeholder */
	SLATE_ARGUMENT(FText, Text)
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
	// Construct the inner contents
	virtual TSharedPtr<SWidget> ConstructContents() override;

	FText Text;

	// Card text widget
	TSharedPtr<STextBlock> TxtText;

public:
	void UpdateText(FText NewText);

};
