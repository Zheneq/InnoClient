// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoCards.h"
#include "InnoClient.h"
#include "JsonObjectConverter.h"
#include "AssetRegistryModule.h"
#include "Package.h"
#include "SecureHash.h"
#include "InnoFunctionLibrary.h"
#include "InnoCardData.h"

const FInnoCard UInnoCards::CardNone = FInnoCard();
const FString UInnoCards::CardInfoPath = TEXT("/Game/Data/CardsData");
const FString UInnoCards::CardInfoName = TEXT("CardsData");

UInnoCards::UInnoCards()
{
	bLoaded = false;
	// Load(); // will it ever succeed?
}

bool UInnoCards::Load()
{
	UInnoCardData* MyAsset = Cast<UInnoCardData>(FStringAssetReference(CardInfoPath).TryLoad());

	if (MyAsset)
	{
		Hash = MyAsset->Hash;
		Cards = MyAsset->Cards;
		bLoaded = true;
		return true;
	}
	return false;
}

bool UInnoCards::Save() const
{
	if (!bLoaded) return false;

	UPackage* Package = CreatePackage(nullptr, *CardInfoPath);
	Package->FullyLoad();

	UInnoCardData* Asset = NewObject<UInnoCardData>(Package, *CardInfoName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	Asset->AddToRoot();

	Asset->Hash = Hash;
	Asset->Cards = Cards;

	Package->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(Asset);

	const FString PackageFileName = FPackageName::LongPackageNameToFilename(CardInfoPath, FPackageName::GetAssetPackageExtension());
	bool Result = UPackage::SavePackage(Package, Asset, RF_Public | RF_Standalone, *PackageFileName);

	UE_LOG(LogTemp, Log, TEXT("UInnoCards::Save: New cards are %ssaved."), Result ? TEXT("") : TEXT("not "));
	return Result;
}

const FInnoCard& UInnoCards::GetCard(int32 Index)
{
	check(LoadIfNeeded());

	if (Cards.IsValidIndex(Index))
	{
		return Cards[Index];
	}
	else
	{
		return CardNone;
	}
}

const TArray<FInnoCard>& UInnoCards::GetCards()
{
	return Cards;
}

void UInnoCards::Update(const FString& CardsJson)
{
	const FString NewHash = FMD5::HashAnsiString(*CardsJson);

	if (NewHash != Hash)
	{
		TArray<TSharedPtr<FJsonValue> > JsonArray;
		TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(CardsJson);
		if (!FJsonSerializer::Deserialize(JsonReader, JsonArray))
		{
			UE_LOG(LogJson, Warning, TEXT("UInnoCards::Update - Unable to parse."), *CardsJson);
			return;
		}


		bLoaded = false; // in case we fail
		Cards.Empty(JsonArray.Num());

		int32 i = 0;
		for (const auto& C : JsonArray)
		{
			FInnoCardRaw Card;
			if (C->IsNull() || FJsonObjectConverter::JsonObjectToUStruct<FInnoCardRaw>(C->AsObject().ToSharedRef(), &Card, 0, 0))
			{
				Cards.Add(ConvertCard(Card, i++));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("UInnoCards::Update failed."));
				return;
			}
		}

		Hash = NewHash;
		bLoaded = true;
		Save();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UInnoCards::Update: Cards are up to date."));
	}

}

FText MakeText(const FString& RawString, const FString& Namespace, const FString Key)
{
	const FString ds = UInnoFunctionLibrary::DeHTML(RawString);
	const FText dt = FText::FromString(ds);

#if WITH_EDITOR
	return FText::ChangeKey(Namespace, Key + TEXT("_") + ds, dt);
#endif

	return dt;
}

FInnoCard UInnoCards::ConvertCard(const FInnoCardRaw& RawCard, int32 Index) const
{
	FInnoCard Res;
	const auto NS = FString::Printf(TEXT("InnoCard-%06d"), Index);

	Res.Age = RawCard.Age;
	Res.Color = UInnoFunctionLibrary::ColorFromString(this, RawCard.Color);
	Res.Name = MakeText(RawCard.Name, NS, TEXT("card_name"));

	for (const auto& i : RawCard.Icons)
	{
		Res.Icons.Add(UInnoFunctionLibrary::ResourceFromString(this, i));
	}
	int32 i = 0;
	for (const auto& d : RawCard.Dogmas)
	{
		Res.Dogmas.Add(MakeText(d, NS, FString::Printf(TEXT("dogma_%d"), i++)));
	}
	i = 0;
	for (const auto& d : RawCard.Echoes)
	{
		Res.Echoes.Add(MakeText(d, NS, FString::Printf(TEXT("echo_%d"), i++)));
	}
	for (const auto& d : RawCard.Inspire)
	{
		Res.Inspire.Add(MakeText(d, NS, FString::Printf(TEXT("inspire_%d"), i++)));
	}
	Res.Karma = MakeText(RawCard.Karmatext, NS, TEXT("karma"));
	Res.Text = MakeText(RawCard.Text, NS, TEXT("text"));
	Res.Set = RawCard.Set;

	if (RawCard.Dogmas.IsValidIndex(0))
	{
		int32 start = RawCard.Dogmas[0].Find(TEXT("inline-"));
		int32 end = RawCard.Dogmas[0].Find(TEXT(".png"));
		if (start != INDEX_NONE && end != INDEX_NONE)
		{
			Res.DogmaResource = UInnoFunctionLibrary::ResourceFromString(this, RawCard.Dogmas[0].Mid(start + 7, end - (start + 7)));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UInnoCards::ConvertCard: Could not retireve dogma resource."));
		}
	}

	return Res;
}


