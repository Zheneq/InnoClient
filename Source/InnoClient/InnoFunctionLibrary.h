// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno", Meta = (WorldContext = "WorldContextObject"))
		static EInnoResource ResourceFromString(const UObject* WorldContextObject, FString String);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static int32 SplayStart(EInnoSplay Splay);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static int32 SplayEnd(EInnoSplay Splay);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno", Meta = (WorldContext = "WorldContextObject"))
		static const FInnoCard& GetCard(const UObject* WorldContextObject, int32 Index);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static FString GetInlineScriptVar(FString Page, FString VarName);

	static bool ParseJson(FString SerializedJson, TSharedPtr<FJsonValue>& JsonArray);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static FString DeHTML(FString String);
	
};
