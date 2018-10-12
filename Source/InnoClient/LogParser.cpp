// Fill out your copyright notice in the Description page of Project Settings.

#include "LogParser.h"
#include "GMInno.h"
#include "Regex.h"
#include "Engine.h"

void ULogParser::Init(AGMInno* GM)
{
	GM->Log.AddDynamic(this, &ULogParser::OnLogMessage);
}

void ULogParser::RegisterCardContainer(UWidget* Container, FName Tag)
{
	if (Container)
	{
		const auto ArrayPtr = Containers.Find(Tag);
		if (ArrayPtr)
		{
			ArrayPtr->Add(Container);
		}
	}
}

void ULogParser::UnregisterCardContainer(UWidget* Container, FName Tag)
{
	if (Container)
	{
		const auto ArrayPtr = Containers.Find(Tag);
		if (ArrayPtr)
		{
			ArrayPtr->Remove(Container);
		}
	}
}

TWeakObjectPtr<UWidget> ULogParser::GetBestContainerForTag(FName Tag)
{
	const auto ArrayPtr = Containers.Find(Tag);
	if (ArrayPtr)
	{
		// Remove all nullptrs
		// ArrayPtr->RemoveAll([](const UWidget* Ptr) { return Ptr == nullptr;	});

		for (auto It = ArrayPtr->CreateIterator(); It; ++It)
		{
			if (*It == nullptr)
			{
				It.RemoveCurrent();
			}
			else
			{
				return *It;
			}
		}
	}

	return nullptr;
}

#define REGEX_SUBLEVEL TEXT("... ")
#define REGEX_CARD_NAME TEXT("<span cardnum=\\\"(\\d+)\\\"[^<]+</span>")
#define REGEX_CARD_SET_AGE TEXT("(a|two|three|four|five|six|seven|eight|nine|ten){0,1} *<span class=\\\"age ([befc])[^>]+>(\\d+)</span>s*")
#define REGEX_CARD TEXT("(?:") REGEX_CARD_NAME TEXT("){0,1}") REGEX_CARD_SET_AGE
#define REGEX_LAST_CARD TEXT("(?:(?: *and *| *\\, *){0,1}(?:") REGEX_CARD_NAME TEXT("){0,1}") REGEX_CARD_SET_AGE TEXT(")+")
#define REGEX_NICK TEXT("(\\S+) ")
#define REGEX_ACTION TEXT("(meld|tuck|score|foreshadow|reveal)s{0,1} ")
#define REGEX_DRAW_AND_DO TEXT("draws{0,1} and ") REGEX_ACTION
#define REGEX_PLACE TEXT("(hand|board|score pile)")
#define REGEX_OWNER TEXT("(your){0,1}(his|her|their){0,1}(") REGEX_NICK TEXT("'s) ")

void ULogParser::OnLogMessage(const FString& String, bool bReplaceLastMessage)
{
	FString MyString = String;

	int32 Level = 0;
	while (MyString.RemoveFromStart(REGEX_SUBLEVEL)) ++Level;

	struct CardDesc
	{
		int32 CardId;
		FString Number;
		FString Set;
		int32 Age;

		CardDesc() : CardId(0), Age(0) {}
		CardDesc(int32 _CardId, FString _Number, FString _Set, int32 _Age) : CardId(_CardId), Number(_Number), Set(_Set), Age(_Age) {}
	};

	TArray<CardDesc> Cards;
	{
		const FRegexPattern RegexCard(REGEX_CARD);
		FRegexMatcher Matcher(RegexCard, String);
		while (Matcher.FindNext())
		{
			const int32 Card = FCString::Atoi(*Matcher.GetCaptureGroup(1));
			const FString Number = Matcher.GetCaptureGroup(2);
			const FString Set = Matcher.GetCaptureGroup(3);
			const int32 Age = FCString::Atoi(*Matcher.GetCaptureGroup(4));

			Cards.Emplace(Card, Number, Set, Age);
		}
	}

	FString SourceTag, DestTag;
	{
		const FRegexPattern RegexSource(TEXT("from ") REGEX_OWNER REGEX_PLACE);
		FRegexMatcher Matcher(RegexSource, String);
		if (Matcher.FindNext())
		{
			const bool bMe      = !Matcher.GetCaptureGroup(1).IsEmpty();
			const bool bThey    = !Matcher.GetCaptureGroup(2).IsEmpty();
			const FString Owner =  Matcher.GetCaptureGroup(3);
			const FString Place =  Matcher.GetCaptureGroup(4);

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 50, FColor::Yellow, FString::Printf(TEXT("from %s %s"), bMe ? TEXT("my") : bThey ? TEXT("their") : *Owner, *Place));

		}
	}
	{
		const FRegexPattern RegexDest(TEXT("to ") REGEX_OWNER REGEX_PLACE);
		FRegexMatcher Matcher(RegexDest, String);
		if (Matcher.FindNext())
		{
			const bool bMe      = !Matcher.GetCaptureGroup(1).IsEmpty();
			const bool bThey    = !Matcher.GetCaptureGroup(2).IsEmpty();
			const FString Owner =  Matcher.GetCaptureGroup(3);
			const FString Place =  Matcher.GetCaptureGroup(4);

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 50, FColor::Yellow, FString::Printf(TEXT("to %s %s"), bMe ? TEXT("my") : bThey ? TEXT("their") : *Owner, *Place));

		}
	}

	FString CardsString;
	for (const auto& C : Cards)
	{
		CardsString.Append(FString::Printf(TEXT("%s [%d|%s%d] "), *C.Number, C.CardId, *C.Set, C.Age));
	}
	
	const FRegexPattern RegexMeldGoFirst(REGEX_NICK TEXT("melds{0,1} ") REGEX_CARD TEXT(" and go(?:es){0,1} first."));
	const FRegexPattern RegexDrawAndDo(REGEX_NICK REGEX_DRAW_AND_DO);
	const FRegexPattern RegexDraw(REGEX_NICK TEXT("draws{0,1} ")); // ...as a sharing bonus
	const FRegexPattern RegexEndorse(REGEX_NICK TEXT("endorses{0,1} with "));
	const FRegexPattern RegexEndorsedDogma(REGEX_NICK TEXT("activates{0,1} the dogma of ") REGEX_CARD TEXT(" with endorsement."));
	const FRegexPattern RegexDogma(REGEX_NICK TEXT("activates{0,1} the dogma of "));
	const FRegexPattern RegexAchieveRegular(REGEX_NICK TEXT("achieves{0,1} age <span class=age(\\d+)")); // ages 0 to 8 !!
	const FRegexPattern RegexAchieveSpecial(REGEX_NICK TEXT("achieves{0,1} ") REGEX_CARD_NAME);
	const FRegexPattern RegexDecree(REGEX_NICK TEXT("issues{0,1} a ") REGEX_CARD_NAME);
	const FRegexPattern RegexRemove(REGEX_NICK TEXT("removes{0,1} from the game "));
	const FRegexPattern RegexReturn(REGEX_NICK TEXT("returns{0,1} "));
	const FRegexPattern RegexAction(REGEX_NICK REGEX_ACTION);

	{
		FRegexMatcher Matcher(RegexMeldGoFirst, String); 
		if (Matcher.FindNext())
		{
			const FString Nick = Matcher.GetCaptureGroup(1);
			const int32 Card = FCString::Atoi(*Matcher.GetCaptureGroup(2));

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 50, FColor::Yellow, FString::Printf(TEXT("Meld and go first: %s - %s"), *Nick, *CardsString));

			return;
		}
	}

	{
		FRegexMatcher Matcher(RegexDrawAndDo, String);
		if (Matcher.FindNext())
		{
			const FString Nick = Matcher.GetCaptureGroup(1);
			const FString Action = Matcher.GetCaptureGroup(2);
			const int32 Card = FCString::Atoi(*Matcher.GetCaptureGroup(3));
			const FString Set = Matcher.GetCaptureGroup(5);
			const int32 Age = FCString::Atoi(*Matcher.GetCaptureGroup(6));

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 50, FColor::Yellow, FString::Printf(TEXT("Action: %s - draw and %s - %s"), *Nick, *Action, *CardsString));

			return;
		}
	}

	{
		FRegexMatcher Matcher(RegexDraw, String);
		if (Matcher.FindNext())
		{
			const FString Nick = Matcher.GetCaptureGroup(1);
			const int32 Card = FCString::Atoi(*Matcher.GetCaptureGroup(2));
			const FString Set = Matcher.GetCaptureGroup(4);
			const int32 Age = FCString::Atoi(*Matcher.GetCaptureGroup(5));

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 50, FColor::Yellow, FString::Printf(TEXT("Draw: %s - %s"), *Nick, *CardsString));

			return;
		}
	}

	{
		FRegexMatcher Matcher(RegexEndorse, String);
		if (Matcher.FindNext())
		{
			const FString Nick = Matcher.GetCaptureGroup(1);
			const int32 Card = FCString::Atoi(*Matcher.GetCaptureGroup(2));

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 50, FColor::Yellow, FString::Printf(TEXT("Endorse: %s - with %s"), *Nick, *CardsString));

			return;
		}

	}

	{
		FRegexMatcher Matcher(RegexEndorsedDogma, String);
		if (Matcher.FindNext())
		{
			const FString Nick = Matcher.GetCaptureGroup(1);
			const int32 Card = FCString::Atoi(*Matcher.GetCaptureGroup(2));

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 50, FColor::Yellow, FString::Printf(TEXT("Endorse: %s - %s"), *Nick, *CardsString));

			return;
		}

	}

	{
		FRegexMatcher Matcher(RegexDogma, String);
		if (Matcher.FindNext())
		{
			const FString Nick = Matcher.GetCaptureGroup(1);
			const int32 Card = FCString::Atoi(*Matcher.GetCaptureGroup(2));

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 50, FColor::Yellow, FString::Printf(TEXT("Dogma: %s - %s"), *Nick, *CardsString));

			return;
		}

	}

	{
		FRegexMatcher Matcher(RegexAchieveRegular, String);
		if (Matcher.FindNext())
		{
			const FString Nick = Matcher.GetCaptureGroup(1);
			const int32 Age = FCString::Atoi(*Matcher.GetCaptureGroup(2)) + 1;

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 50, FColor::Yellow, FString::Printf(TEXT("Achieve: %s - age %d"), *Nick, Age));

			return;
		}

	}

	{
		FRegexMatcher Matcher(RegexAchieveSpecial, String);
		if (Matcher.FindNext())
		{
			const FString Nick = Matcher.GetCaptureGroup(1);
			const int32 Card = FCString::Atoi(*Matcher.GetCaptureGroup(2));

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 50, FColor::Yellow, FString::Printf(TEXT("Achieve: %s - spec. card %d"), *Nick, Card));

			return;
		}

	}

	{
		FRegexMatcher Matcher(RegexDecree, String);
		if (Matcher.FindNext())
		{
			const FString Nick = Matcher.GetCaptureGroup(1);
			const int32 Card = FCString::Atoi(*Matcher.GetCaptureGroup(2));

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 50, FColor::Yellow, FString::Printf(TEXT("Decree: %s - spec. card %d"), *Nick, Card));

			return;
		}

	}

	{
		FRegexMatcher Matcher(RegexRemove, String);
		if (Matcher.FindNext())
		{
			const FString Nick = Matcher.GetCaptureGroup(1);
			const int32 Card = FCString::Atoi(*Matcher.GetCaptureGroup(2));
			const FString Set = Matcher.GetCaptureGroup(4);
			const int32 Age = FCString::Atoi(*Matcher.GetCaptureGroup(5));

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 50, FColor::Yellow, FString::Printf(TEXT("Remove: %s - %s"), *Nick, *CardsString));

			return;
		}

	}

	{
		FRegexMatcher Matcher(RegexReturn, String);
		if (Matcher.FindNext())
		{
			const FString Nick = Matcher.GetCaptureGroup(1);
			const int32 Card = FCString::Atoi(*Matcher.GetCaptureGroup(2));
			const FString Set = Matcher.GetCaptureGroup(4);
			const int32 Age = FCString::Atoi(*Matcher.GetCaptureGroup(5));

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 50, FColor::Yellow, FString::Printf(TEXT("Return: %s - %s"), *Nick, *CardsString));

			return;
		}

	}

	{
		FRegexMatcher Matcher(RegexAction, String);
		if (Matcher.FindNext())
		{
			const FString Nick = Matcher.GetCaptureGroup(1); 
			const FString Action = Matcher.GetCaptureGroup(2);
			const int32 Card = FCString::Atoi(*Matcher.GetCaptureGroup(3));
			const FString Set = Matcher.GetCaptureGroup(5);
			const int32 Age = FCString::Atoi(*Matcher.GetCaptureGroup(6)); 

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 50, FColor::Yellow, FString::Printf(TEXT("Action: %s - %s - %s"), *Nick, *Action, *CardsString));

			return;
		}
	}
}

#undef REGEX_SUBLEVEL
#undef REGEX_CARD_NAME
#undef REGEX_CARD_SET_AGE
#undef REGEX_CARD
#undef REGEX_LAST_CARD
#undef REGEX_NICK
#undef REGEX_ACTION
#undef REGEX_DRAW_AND_DO
#undef REGEX_PLACE
#undef REGEX_OWNER
