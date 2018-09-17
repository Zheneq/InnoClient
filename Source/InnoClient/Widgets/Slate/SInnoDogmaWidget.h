#pragma once

#include "SlateBasics.h"

// Lays out and controls the Main Menu UI for our tutorial.
class INNOCLIENT_API SInnoDogmaWidget : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SInnoDogmaWidget)
	{}
	SLATE_ARGUMENT(FText, Text)
	SLATE_END_ARGS()

	// Constructs and lays out the Main Menu UI Widget.
	// args Arguments structure that contains widget-specific setup information.
	void Construct(const FArguments& args);

	// Localized text split into words
	TArray<FString> Words;

	TAttribute<const FText*> Text;
	void SetText(const TAttribute<const FText*>& InText);

	const struct FInnoDogmaStyle* DogmaStyle;

protected:
	void RebuildWidget();
	void ParseText(const TAttribute<const FText*>& InText);
};