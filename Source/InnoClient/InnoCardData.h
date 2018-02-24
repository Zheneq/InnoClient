// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InnoCards.h"
#include "InnoCardData.generated.h"

/**
 * 
 */
UCLASS()
class INNOCLIENT_API UInnoCardData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inno")
		FString Hash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inno")
		TArray<FInnoCard> Cards;

};
