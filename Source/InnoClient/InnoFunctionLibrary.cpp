// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoFunctionLibrary.h"
#include "InnoClient.h"


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
			return String;
		}
	}
}