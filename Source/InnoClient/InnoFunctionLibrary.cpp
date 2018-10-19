// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoFunctionLibrary.h"
#include "InnoClient.h"
#include "GIInno.h"
#include "InnoCardData.h"
#include "Regex.h"


EInnoColor UInnoFunctionLibrary::ColorFromString(const UObject* WorldContextObject, FString String)
{
	auto World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UGIInno* GI = World ? Cast<UGIInno>(World->GetGameInstance()) : nullptr;

	return (GI && GI->ColorFromString.Contains(String)) ? GI->ColorFromString[String] : EInnoColor::IC_None;
}

const FString& UInnoFunctionLibrary::StringFromColor(EInnoColor InColor)
{
	static const FString Red    = FString("red");
	static const FString Green  = FString("green");
	static const FString Blue   = FString("blue");
	static const FString Purple = FString("purple");
	static const FString Yellow = FString("yellow");
	static const FString Def    = FString("");

	switch (InColor)
	{
	case EInnoColor::IC_Red:
		return Red;
	case EInnoColor::IC_Green:
		return Green;
	case EInnoColor::IC_Blue:
		return Blue;
	case EInnoColor::IC_Purple:
		return Purple;
	case EInnoColor::IC_Yellow:
		return Yellow;
	default:
		return Def;
	}
}

EInnoResource UInnoFunctionLibrary::ResourceFromString(const UObject* WorldContextObject, FString String)
{
	auto World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UGIInno* GI = World ? Cast<UGIInno>(World->GetGameInstance()) : nullptr;

	return (GI && GI->ResourceFromString.Contains(String)) ? GI->ResourceFromString[String] : EInnoResource::IR_Hex;
}

int32 UInnoFunctionLibrary::SplayStart(EInnoSplay Splay)
{
	switch (Splay)
	{
	case EInnoSplay::IPA_Left:
		return 3;
	case EInnoSplay::IPA_Right:
		return 0;
	case EInnoSplay::IPA_Up:
		return 1;
	default:
		return -1;
	}
}

int32 UInnoFunctionLibrary::SplayEnd(EInnoSplay Splay)
{
	switch (Splay)
	{
	case EInnoSplay::IPA_Left:
		return 5;
	case EInnoSplay::IPA_Right:
		return 2;
	case EInnoSplay::IPA_Up:
		return 4;
	default:
		return -1;
	}
}

const FInnoCard& UInnoFunctionLibrary::GetCard(const UObject* WorldContextObject, int32 Index)
{
	auto World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	AGMInno* GM = World ? Cast<AGMInno>(World->GetAuthGameMode()) : nullptr;

	if (GM)
	{
		return GM->Cards->GetCard(Index);
	}

#if WITH_EDITOR
	// No gamemode, fall back to data asset
	UInnoCardData* MyAsset = Cast<UInnoCardData>(FStringAssetReference(UInnoCards::CardInfoPath).TryLoad());

	if (MyAsset && MyAsset->Cards.IsValidIndex(Index))
	{
		return MyAsset->Cards[Index];
	}
#endif // WITH_EDITOR

	return UInnoCards::CardNone;
}

FString UInnoFunctionLibrary::GetInlineScriptVar(FString Page, FString VarName)
{
	int32 NameIdx = Page.Find(VarName);
	if (NameIdx == INDEX_NONE)
	{
		return TEXT("");
	}

	int32 VarIdx = Page.Find(TEXT("="), ESearchCase::IgnoreCase, ESearchDir::FromStart, NameIdx) + 1;
	int32 VarEnd = Page.Find(TEXT(";"), ESearchCase::IgnoreCase, ESearchDir::FromStart, VarIdx);

	if (VarIdx == INDEX_NONE || VarEnd == INDEX_NONE)
	{
		return TEXT("");
	}

	FString Result = Page.Mid(VarIdx, VarEnd - VarIdx).TrimStart().TrimEnd().TrimQuotes();

	// TrimQuotes does not remove single quotes
	if (Result[0] == '\'' || Result[Result.Len() - 1] == '\'')
	{
		Result = Result.Mid(1, Result.Len() - 2);
	}

	return Result;
}

bool UInnoFunctionLibrary::ParseJson(FString SerializedJson, TSharedPtr<FJsonValue>& JsonArray)
{
	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(SerializedJson);

	//Deserialize the json data given Reader and the actual object to deserialize
	return FJsonSerializer::Deserialize(Reader, JsonArray);
}

#define REGEX_CARD_NAME TEXT("<span cardnum=\\\"(\\d+)\\\"[^>]+>([^<]+)</span>")
#define REGEX_CARD_SET_AGE TEXT("<span class=\\\"age ([befc])[^>]+>(\\d+)</span>s*")
#define REGEX_CARD TEXT("(?:") REGEX_CARD_NAME TEXT("){0,1}") REGEX_CARD_SET_AGE
#define REGEX_AGE TEXT("<span class=(?:\\\"){0,1}age[^>]+>(\\d+)</span>")
#define REGEX_ICON TEXT("<img src=\\\"/static/icons/inline\\-([^.]+).png\\\">")

FORCEINLINE void ReplaceMatch(FString& String, FRegexMatcher& Matcher, const FString& Replace)
{
	String = String.Left(Matcher.GetCaptureGroupBeginning(0)) + Replace + String.Mid(Matcher.GetCaptureGroupEnding(0));
}

// TODO split this function? one for cards, one for log
FString UInnoFunctionLibrary::DeHTML(FString String)
{
	// Cards in log
	{
		const FRegexPattern RegexCard(REGEX_CARD);
		while (true)
		{
			FRegexMatcher Matcher(RegexCard, String);
			if (Matcher.FindNext())
			{
				const int32 Card = FCString::Atoi(*Matcher.GetCaptureGroup(1));
				const FString Name = Matcher.GetCaptureGroup(2);
				const FString Set = Matcher.GetCaptureGroup(3);
				const int32 Age = FCString::Atoi(*Matcher.GetCaptureGroup(4));

				if (Card)
				{
					// ReplaceMatch(String, Matcher, FString::Printf("<Inno.InlineCard>%d</>", Card));
					ReplaceMatch(String, Matcher, FString::Printf(TEXT("[%s (%s%d)]"), *Name, *Set, Age));
				}
				else
				{
					ReplaceMatch(String, Matcher, FString::Printf(TEXT("[%s%d]"), *Set, Age));
				}
			}
			else
			{
				break;
			}
		}
	}

	// Replace age tags
	{
		const FRegexPattern Regex(REGEX_AGE);
		FRegexMatcher Matcher(Regex, String);
		if (Matcher.FindNext())
		{
			const int32 Age = FCString::Atoi(*Matcher.GetCaptureGroup(1));
			ReplaceMatch(String, Matcher, FString::Printf(TEXT("{%s}"), Age));
		}
	}

	// Replace resource icon tags
	{
		const FRegexPattern RegexIcon(REGEX_ICON);
		while (true)
		{
			FRegexMatcher Matcher(RegexIcon, String);
			if (Matcher.FindNext())
			{
				const FString Icon = Matcher.GetCaptureGroup(1);
				ReplaceMatch(String, Matcher, FString::Printf(TEXT("{%s}"), *Icon));
			}
			else
			{
				break;
			}
		}
	}

	// Remove all other tags
	{
		const FRegexPattern Regex(TEXT("<([^>]+)>"));
		const FString EmptyString;
		while (true)
		{
			FRegexMatcher Matcher(Regex, String);
			if (Matcher.FindNext())
			{
				ReplaceMatch(String, Matcher, EmptyString);
			}
			else
			{
				break;
			}
		}
	}

	// Replace brackets with RichTextBlock tags
	{
		const FRegexPattern Regex(TEXT("\\{([^\\}]+)\\}"));
		while (true)
		{
			FRegexMatcher Matcher(Regex, String);
			if (Matcher.FindNext())
			{
				const FString Name = Matcher.GetCaptureGroup(1);
				ReplaceMatch(String, Matcher, FString::Printf(TEXT("<img src=\"%s\"/>"), *Name));
			}
			else
			{
				break;
			}
		}
	}
	
	// Clean the string
	String.ReplaceInline(TEXT("&mdash;"), TEXT("~"));
	String.ReplaceInline(TEXT("&ndash;"), TEXT("-"));
	String.ReplaceInline(TEXT("&lt;"), TEXT("<"));
	String.ReplaceInline(TEXT("&gt;"), TEXT(">"));
	String.ReplaceInline(TEXT("."), TEXT(". "));
	String.ReplaceInline(TEXT("\n"), TEXT(" "));
	String.ReplaceInline(TEXT("\r"), TEXT(" "));
	String.ReplaceInline(TEXT("\t"), TEXT(" "));
	while (String.ReplaceInline(TEXT("  "), TEXT(" ")));

	return String;
}

#undef REGEX_CARD_NAME
#undef REGEX_CARD_SET_AGE
#undef REGEX_CARD
#undef REGEX_AGE
#undef REGEX_ICON
