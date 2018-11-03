#pragma once

#include "SlateBasics.h"
#include "InnoCards.h"
#include "Widgets/Styles/InnoLogStyle.h"
#include "Widgets/Styles/InnoCardWidgetStyle.h"
#include "Widgets/Styles/InnoWidgetStyles.h"

class INNOCLIENT_API SInnoGameLog : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SInnoGameLog)
		: _Style(&FInnoWidgetStyles::Get().GetWidgetStyle<FInnoLogStyle>("Style_InnoLog"))
		, _CardStyle(&FInnoWidgetStyles::Get().GetWidgetStyle<FInnoCardStyle>("Style_InnoCard"))
	{}
	/** Style override. If not set, default one will be used. */
	SLATE_STYLE_ARGUMENT(FInnoLogStyle, Style)
	/** Style override. If not set, default one will be used. 
	* Used for coloring inline widget and for popup card.
	*/
	SLATE_STYLE_ARGUMENT(FInnoCardStyle, CardStyle)
	SLATE_END_ARGS()

	void Construct(const FArguments& args);

protected:
	// Style currently in use. Pass style ptr in construct if you want to override the default one.
	const FInnoLogStyle* Style;

	// Card style for coloring inline widget and for popup card.
	const FInnoCardStyle* CardStyle;

	// Outer scrollbox.
	TSharedPtr<SScrollBox> SB;

	// Container.
	// We cannot put our messages into the scrollbox directly as it does not let us access its children.
	TSharedPtr<SVerticalBox> VB;

	// Cached baseline for our font.
	int16 Baseline;

	// Current numer of messages
	int32 MessageNum;


public:
	// Create inline widget.
	FSlateWidgetRun::FWidgetRunInfo OnCreateWidgetDecoratorWidget(const FTextRunInfo& RunInfo, const ISlateStyle* Style) const;

	// Create card popup.
	FReply OnCardNameClicked();

	// Add new message (and remove the oldest one if the time has come)
	void AddMessage(const FText& Message, bool bReplaceLastMessage = false);

	// Clear all text
	void Clear();
};
