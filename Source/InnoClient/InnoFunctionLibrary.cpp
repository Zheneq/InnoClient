// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoFunctionLibrary.h"
#include "InnoClient.h"
#include "GIInno.h"
#include "GMInno.h"
#include "InnoCardData.h"


EInnoColor UInnoFunctionLibrary::ColorFromString(const UObject* WorldContextObject, FString String)
{
	auto World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UGIInno* GI = World ? Cast<UGIInno>(World->GetGameInstance()) : nullptr;

	return (GI && GI->ColorFromString.Contains(String)) ? GI->ColorFromString[String] : EInnoColor::IC_None;
}

EInnoResource UInnoFunctionLibrary::ResourceFromString(const UObject* WorldContextObject, FString String)
{
	auto World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UGIInno* GI = World ? Cast<UGIInno>(World->GetGameInstance()) : nullptr;

	return (GI && GI->ResourceFromString.Contains(String)) ? GI->ResourceFromString[String] : EInnoResource::IR_Hex;
}

const FInnoCard& UInnoFunctionLibrary::GetCard(const UObject* WorldContextObject, int32 Index)
{
	auto World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	AGMInno* GM = World ? Cast<AGMInno>(World->GetAuthGameMode()) : nullptr;

	if (GM)
	{
		return GM->Cards->GetCard(Index);
	}

#ifdef WITH_EDITOR
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

FString UInnoFunctionLibrary::DeHTML(FString String)
{
	int32 start, end;

	// Replace age tags
	String.ReplaceInline(TEXT("<span class=age>"), TEXT("["));
	String.ReplaceInline(TEXT("</span>"), TEXT("]"));

	// Replace resource icon tags
	while (true)
	{
		String.FindChar('<', start);
		end = String.Find(TEXT(">"), ESearchCase::IgnoreCase, ESearchDir::FromStart, start);

		if (start != INDEX_NONE && end != INDEX_NONE)
		{
			const int32 len = end - start + 1;

			FString TagToRemove = String.Mid(start, len);

			// If the tag contains a resource picture
			const FString InlinePicLeft = TEXT("<img src=\"/static/icons/inline-");
			const FString InlinePicRight = TEXT(".png\">");
			if (TagToRemove.StartsWith(InlinePicLeft) && TagToRemove.EndsWith(InlinePicRight))
			{
				FString Replace = FString::Printf(TEXT("[%s]"), *TagToRemove.Mid(InlinePicLeft.Len(), TagToRemove.Len() - InlinePicRight.Len() - InlinePicLeft.Len()));
				String.InsertAt(end + 1, Replace);
			}

			String.RemoveAt(start, len);
		}
		else
		{
			break;
		}
	}

	// Clean the string
	String.ReplaceInline(TEXT("."), TEXT(". "));
	String.ReplaceInline(TEXT("\n"), TEXT(" "));
	String.ReplaceInline(TEXT("\r"), TEXT(" "));
	String.ReplaceInline(TEXT("\t"), TEXT(" "));
	while (String.ReplaceInline(TEXT("  "), TEXT(" ")));

	return String;
}