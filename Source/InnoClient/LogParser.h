// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UserWidget.h"
#include "LogParser.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class INNOCLIENT_API ULogParser : public UObject
{
	GENERATED_BODY()

private:
	TMap<FName, TArray<UWidget*>> Containers;

public:
	/*  */
	UFUNCTION(BlueprintCallable)
		void Init(class AGMInno* GM);

	/*  */
	UFUNCTION(BlueprintCallable)
		void RegisterCardContainer(UWidget* Container, FName Tag);

	/*  */
	UFUNCTION(BlueprintCallable)
		void UnregisterCardContainer(UWidget* Container, FName Tag);

	/*  */
	UFUNCTION(BlueprintCallable)
		void OnLogMessage(const FString& String, bool bReplaceLastMessage);

private:
	TWeakObjectPtr<UWidget> GetBestContainerForTag(FName Tag);
};
