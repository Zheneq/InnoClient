// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InnoCards.generated.h"

UENUM(BlueprintType)
enum class EInnoColor : uint8
{
	IC_None 	UMETA(DisplayName = "None"),
	IC_Red	 	UMETA(DisplayName = "Red"),
	IC_Yellow 	UMETA(DisplayName = "Yellow"),
	IC_Green 	UMETA(DisplayName = "Green"),
	IC_Purple 	UMETA(DisplayName = "Purple"),
	IC_Blue 	UMETA(DisplayName = "Blue"),
};

UENUM(BlueprintType)
enum class EInnoResource : uint8
{
	IR_Hex		 	UMETA(DisplayName = "Hex"),
	IR_Castle 		UMETA(DisplayName = "Castle"),
	IR_Crown		UMETA(DisplayName = "Crown"),
	IR_Leaf			UMETA(DisplayName = "Leaf"),
	IR_Lightbulb 	UMETA(DisplayName = "Lightbulb"),
	IR_Factory		UMETA(DisplayName = "Factory"),
	IR_Clock		UMETA(DisplayName = "Clock"),
	IR_Echo 		UMETA(DisplayName = "Echo"),
	IR_Inspire		UMETA(DisplayName = "Inspire"),
	IR_Left 		UMETA(DisplayName = "Left"),
	IR_Right 		UMETA(DisplayName = "Right"),
	IR_Up	 		UMETA(DisplayName = "Up"),
	IR_Whiteflag	UMETA(DisplayName = "Whiteflag"),
	IR_BlackFlag	UMETA(DisplayName = "BlackFlag"),
	IR_Plus			UMETA(DisplayName = "Plus"),
	IR_Bonus1	 	UMETA(DisplayName = "Bonus-1"),
	IR_Bonus2	 	UMETA(DisplayName = "Bonus-2"),
	IR_Bonus3	 	UMETA(DisplayName = "Bonus-3"),
	IR_Bonus4	 	UMETA(DisplayName = "Bonus-4"),
	IR_Bonus5	 	UMETA(DisplayName = "Bonus-5"),
	IR_Bonus6	 	UMETA(DisplayName = "Bonus-6"),
	IR_Bonus7	 	UMETA(DisplayName = "Bonus-7"),
	IR_Bonus8	 	UMETA(DisplayName = "Bonus-8"),
	IR_Bonus9	 	UMETA(DisplayName = "Bonus-9"),
	IR_Bonus10	 	UMETA(DisplayName = "Bonus-10"),
	IR_Bonus11	 	UMETA(DisplayName = "Bonus-11"),
	IR_Bonus12	 	UMETA(DisplayName = "Bonus-12"),
};


USTRUCT(BlueprintType)
struct FInnoCard
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 Age;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		EInnoColor Color;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		FText Name;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		TArray<EInnoResource> Icons;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		EInnoResource DogmaResource;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		TArray<FText> Dogmas;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		TArray<FText> Echoes;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		TArray<FText> Inspire;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		FText Karma;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 Set;

	FInnoCard() : Age(-1), Set(-1), Color(EInnoColor::IC_None), DogmaResource(EInnoResource::IR_Hex) {}
};

USTRUCT(BlueprintType)
struct FInnoCardRaw
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 Age;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		FString Color;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		FString Html;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		FString ShortHtml;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		FString Text;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		TArray<FString> Icons;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		TArray<FString> Dogmas;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		TArray<FString> Echoes;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		TArray<FString> Inspire;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		TArray<FString> Names;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		FString Karmatext;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		FString Name;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 Set;

	FInnoCardRaw() : Age(-1), Set(-1) {}
};

/**
 * 
 */
UCLASS()
class INNOCLIENT_API UInnoCards : public UObject
{
	GENERATED_BODY()

	TArray<FInnoCard> Cards;

	const static FInnoCard CardNone;

	FInnoCard ConvertCard(const FInnoCardRaw& RawCard) const;

	EInnoColor ColorFromString(FString String) const;

	EInnoResource ResourceFromString(FString String) const;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		const FInnoCard& GetCard(int32 Index) const;

	UFUNCTION(BlueprintCallable, Category = "Inno")
		void Load(const FString& CardsJson);
	
	
};

