// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SInnoDogmaWidget.h"
#include "InnoWidgetStyles.h"
#include "InnoDogmaStyle.h"
#include "InnoClient.h"
#include "Engine.h"

void SInnoDogmaWidget::Construct(const FArguments& args)
{
	ParseText(&args._Text);

	DogmaStyle = &FInnoWidgetStyles::Get().GetWidgetStyle<FInnoDogmaStyle>("Style_InnoDogma");
	RebuildWidget();
}

//BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION // disables compiler optimization (optimizing complex widgets is a chore for the compiler)
void SInnoDogmaWidget::RebuildWidget()
{
	ChildSlot
		[
			SNew(SWrapBox)
				.UseAllottedWidth(true)
		];

	const TSharedRef<SWrapBox> Box = StaticCastSharedRef<SWrapBox>(ChildSlot.GetWidget());
	DogmaStyle->UpdateBrushes();

	for (int32 i = 0; i < Words.Num(); i += 2)
	{
		if (DogmaStyle->IconBrushes.Contains(Words[i]))
		{
			Box->AddSlot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(EVerticalAlignment::VAlign_Center)
						[
							SNew(SImage)
								.Image(&DogmaStyle->IconBrushes[Words[i]])
						]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(STextBlock)
								.TextStyle(&DogmaStyle->TextStyle)
								.Text(FText::FromString(Words[i+1]))
						]
				];
		}
		else
		{
			Box->AddSlot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				[
					SNew(STextBlock)
						.TextStyle(&DogmaStyle->TextStyle)
						.Text(FText::FromString(Words[i] + Words[i+1]))
				];
		}
	}

}
//END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInnoDogmaWidget::ParseText(const TAttribute<const FText*>& InText)
{
	// Note: If the string starts with a delimiter, it will be omitted

	// InText.Get()->ToString().ParseIntoArrayWS(Words);
	const auto InString = InText.Get()->ToString().Replace(TEXT("\n"), TEXT(" ")).Replace(TEXT("\t"), TEXT(" ")).Replace(TEXT("\r"), TEXT(""));

	static const TCHAR* DelimArray[] =
	{
		TEXT(" "),
		TEXT("\t"),
		TEXT(":"),
		TEXT("."),
		TEXT(","),
		TEXT(";"),
	};

	// start with just the standard whitespaces
	int32 NumDelims = ARRAY_COUNT(DelimArray);

	// Make sure the delimit string is not null or empty
	Words.Empty();
	const TCHAR *Start = InString.GetCharArray().GetData();
	const int32 Length = InString.Len();
	if (Start)
	{
		int32 SubstringBeginIndex = 0;

		// Iterate through string.
		for (int32 i = 0; i < InString.Len();)
		{
			int32 SubstringEndIndex = INDEX_NONE;
			// int32 DelimiterLength = 0;
			const int32 DelimiterLength = 1;

			// Attempt each delimiter.
			for (int32 DelimIndex = 0; DelimIndex < NumDelims; ++DelimIndex)
			{
				// DelimiterLength = FCString::Strlen(DelimArray[DelimIndex]);

				// If we found a delimiter...
				if (FCString::Strncmp(Start + i, DelimArray[DelimIndex], DelimiterLength) == 0)
				{
					// Mark the end of the substring.
					SubstringEndIndex = i;
					break;
				}
			}

			if (SubstringEndIndex != INDEX_NONE)
			{
				const int32 SubstringLength = SubstringEndIndex - SubstringBeginIndex;
				// If we're not culling empty strings or if we are but the string isn't empty anyways...
				if (SubstringLength != 0)
				{
					// ... add new string from substring beginning up to the beginning of this delimiter.
					new (Words) FString(SubstringEndIndex - SubstringBeginIndex, Start + SubstringBeginIndex);
					new (Words) FString(DelimiterLength, Start + SubstringEndIndex);
				}
				else if (Words.Num() > 0)
				{
					// If there are multiple delimitrers in a row, concatenate them
					Words.Last().Append(FString(DelimiterLength, Start + SubstringEndIndex)).ReplaceInline(TEXT("  "), TEXT(" "));
				}
				// Next substring begins at the end of the discovered delimiter.
				SubstringBeginIndex = SubstringEndIndex + DelimiterLength;
				i = SubstringBeginIndex;
			}
			else
			{
				++i;
			}
		}

		// Add any remaining characters after the last delimiter.
		const int32 SubstringLength = Length - SubstringBeginIndex;
		// If we're not culling empty strings or if we are but the string isn't empty anyways...
		if (SubstringLength != 0)
		{
			// ... add new string from substring beginning up to the end...
			new (Words) FString(Start + SubstringBeginIndex);
			// ... and an empty delimiter to keep the number of elements even.
			new (Words) FString();
		}
	}
}

void SInnoDogmaWidget::SetText(const TAttribute<const FText*>& InText)
{
	ParseText(InText);

	FString DebugString(TEXT("["));
	for (const auto& w : Words)
	{
		DebugString.Append(TEXT("<")).Append(w).Append(TEXT(">, "));
	}
	DebugString.Append(TEXT("]"));
	UE_LOG(LogTemp, Log, TEXT("%s"), *DebugString);

	RebuildWidget();
}
