// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/NoExportTypes.h"
#include "InnoCards.h"
#include "GMInno.h"
#include "HAL/PlatformApplicationMisc.h"
#include "InnoFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class INNOCLIENT_API UInnoFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		static void CopyToClipboard(const FString& Text) { FPlatformApplicationMisc::ClipboardCopy(*Text); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static int GetLocalPlayerId() { return -1; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno", Meta = (WorldContext = "WorldContextObject"))
		static EInnoColor ColorFromString(const UObject* WorldContextObject, FString String);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static const FString& StringFromColor(EInnoColor InColor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static const FString& StringFromSet(int32 Set);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static int32 SetFromString(const FString& String);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno", Meta = (WorldContext = "WorldContextObject"))
		static EInnoResource ResourceFromString(const UObject* WorldContextObject, FString String);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static int32 SplayStart(EInnoSplay Splay);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static int32 SplayEnd(EInnoSplay Splay);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno", Meta = (WorldContext = "WorldContextObject"))
		static FInnoCardInfo GetCardInfo(const UObject* WorldContextObject, int32 Index);

	// Returns card index based on card info.
	// May be actual card index, may be unknown card code.
	// Reverse operation to GetCardInfo.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static int32 GetCardIndex(const FInnoCardInfo& Card);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno", Meta = (WorldContext = "WorldContextObject"))
		static const FInnoCard& GetCard(const UObject* WorldContextObject, int32 Index);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static FString GetInlineScriptVar(FString Page, FString VarName);

	static bool ParseJson(FString SerializedJson, TSharedPtr<FJsonValue>& JsonArray);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static FString DeHTML(FString String);

	// Check if this choose option is a card
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static bool IsCard(const FString& String);

	UFUNCTION(BlueprintCallable, Category = "Inno")
		static FInnoCardInfo StringToCard(const FString& String);

};
