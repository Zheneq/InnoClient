// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "InnoCards.h"
#include "InnoFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class INNOCLIENT_API UInnoFunctionLibrary : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		static FString GetInlineScriptVar(FString Page, FString VarName);

	static bool ParseJson(FString SerializedJson, TSharedPtr<FJsonValue>& JsonArray);

	static FString DeHTML(FString String);
	
};
