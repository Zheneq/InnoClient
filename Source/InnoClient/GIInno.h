// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InnoCards.h"
#include "GIInno.generated.h"

/**
 * 
 */
UCLASS()
class INNOCLIENT_API UGIInno : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Inno Low")
		TMap<FString, EInnoResource> ResourceFromString;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inno Low")
		TMap<FString, EInnoColor> ColorFromString;
};
