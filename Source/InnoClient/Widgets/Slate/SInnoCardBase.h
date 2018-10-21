#pragma once

#include "SlateBasics.h"
#include "InnoCards.h"
#include "Widgets/Styles/InnoCardWidgetStyle.h"
#include "Widgets/Styles/InnoWidgetStyles.h"

class SMyBorder;

class INNOCLIENT_API SInnoCardBase : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnCardClicked, FInnoCardInfo);
	DECLARE_DELEGATE_OneParam(FOnCardSimple, FInnoCardInfo);

	SLATE_BEGIN_ARGS(SInnoCardBase)
		: _Style(&FInnoWidgetStyles::Get().GetWidgetStyle<FInnoCardStyle>("Style_InnoCard"))
	{}
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
	// Construct the bacground (just colored card with nothing on it)
	virtual TSharedPtr<SWidget> ConstructCard();
	// Construct the inner contents (override in children)
	virtual TSharedPtr<SWidget> ConstructContents();

protected:
	// Widget wrapper. Sometimes we want to keep minimal height of the card even if there is no text.
	TSharedPtr<SBox> SizeBox;

	// Transparent border for padding and enabling/disabling display of disabled effects
	TSharedPtr<SMyBorder> MBBorder;

	// Border for drawing highlight outline. Does not count into card's size.
	TSharedPtr<SMyBorder> MBHighlight;

	// Colored card background
	TSharedPtr<SBorder> BBackground;

	// Card info. Supposed to be set by children.
	FInnoCardInfo CardInfo;

	// Is card currently highlighted. Can be used e.g. for toggles.
	bool bIsHighlighted;

public:
	FORCEINLINE const FInnoCardInfo& GetCardInfo() const { return CardInfo; }
	FORCEINLINE bool GetIsHighlighted() const { return bIsHighlighted; }
	FORCEINLINE void SetIndex(int32 Index) { CardInfo.Index = Index; } // index only matters once the unknown card appears in a container

	void SetIsHighlighted(bool bNewIsHighlighted);

	// Reset appearance to default.
	virtual void ResetAppearance() {};

protected:
	// Style currently in use. Pass style ptr in construct if you want to override the default one.
	const FInnoCardStyle* Style;

private:
	// Slate events handlers
	FReply OnClickedHandle();
	void OnHoveredHandle();
	void OnUnhoveredHandle();

public:
	/** The delegate to execute when the card is clicked */
	FOnCardClicked OnClicked;

	/** The delegate to execute when the card is hovered */
	FOnCardSimple OnHovered;

	/** The delegate to execute when the card is not hovered anymore */
	FOnCardSimple OnUnhovered;

	/** Unbind all callbacks */
	void UnbindAll()
	{
		OnClicked.Unbind();
		OnHovered.Unbind();
		OnUnhovered.Unbind();
	}

};
