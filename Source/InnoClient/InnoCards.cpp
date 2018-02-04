// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoCards.h"
#include "InnoClient.h"
#include "JsonObjectConverter.h"
#include "InnoFunctionLibrary.h"
#include "GIInno.h"

const FInnoCard UInnoCards::CardNone = FInnoCard();

const FInnoCard& UInnoCards::GetCard(int32 Index) const
{
	if (Cards.IsValidIndex(Index))
	{
		return Cards[Index];
	}
	else
	{
		return CardNone;
	}
}

void UInnoCards::Load(const FString& CardsJson)
{
	TArray<TSharedPtr<FJsonValue> > JsonArray;
	TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(CardsJson);
	if (!FJsonSerializer::Deserialize(JsonReader, JsonArray))
	{
		UE_LOG(LogJson, Warning, TEXT("UInnoCards::Load - Unable to parse."), *CardsJson);
		return;
	}

	for (const auto& C : JsonArray)
	{
		FInnoCardRaw Card;
		if(C->IsNull() || FJsonObjectConverter::JsonObjectToUStruct<FInnoCardRaw>(C->AsObject().ToSharedRef(), &Card, 0, 0))
		{
			Cards.Add(ConvertCard(Card));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UInnoCards::Load failed."));
			return;
		}
	}
}

FInnoCard UInnoCards::ConvertCard(const FInnoCardRaw& RawCard) const
{
	FInnoCard Res;

	Res.Age = RawCard.Age;
	Res.Color = ColorFromString(RawCard.Color);
	Res.Name = FText::FromString(RawCard.Name);
	for (const auto& i : RawCard.Icons)
	{
		Res.Icons.Add(ResourceFromString(i));
	}
	for (const auto& d : RawCard.Dogmas)
	{
		Res.Dogmas.Add(FText::FromString(UInnoFunctionLibrary::DeHTML(d)));
	}
	for (const auto& d : RawCard.Echoes)
	{
		Res.Echoes.Add(FText::FromString(UInnoFunctionLibrary::DeHTML(d)));
	}
	for (const auto& d : RawCard.Inspire)
	{
		Res.Inspire.Add(FText::FromString(UInnoFunctionLibrary::DeHTML(d)));
	}
	Res.Karma = FText::FromString(UInnoFunctionLibrary::DeHTML(RawCard.Karmatext));
	Res.Set = RawCard.Set;

	if (RawCard.Dogmas.IsValidIndex(0))
	{
		int32 start = RawCard.Dogmas[0].Find(TEXT("inline-"));
		int32 end = RawCard.Dogmas[0].Find(TEXT(".png"));
		if (start != INDEX_NONE && end != INDEX_NONE)
		{
			Res.DogmaResource = ResourceFromString(RawCard.Dogmas[0].Mid(start + 7, end - (start + 7)));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UInnoCards::ConvertCard: Could not retireve dogma resource."));
		}
	}

	return Res;
}

EInnoColor UInnoCards::ColorFromString(FString String) const
{
	auto World = GEngine ? GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UGIInno* GI = World ? Cast<UGIInno>(World->GetGameInstance()) : nullptr;

	return (GI && GI->ColorFromString.Contains(String)) ? GI->ColorFromString[String] : EInnoColor::IC_None;
}

EInnoResource UInnoCards::ResourceFromString(FString String) const
{
	auto World = GEngine ? GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UGIInno* GI = World ? Cast<UGIInno>(World->GetGameInstance()) : nullptr;

	return (GI && GI->ResourceFromString.Contains(String)) ? GI->ResourceFromString[String] : EInnoResource::IR_Hex;
}


