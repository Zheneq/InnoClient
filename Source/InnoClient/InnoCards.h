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
	IR_HexNote	 	UMETA(DisplayName = "Hex Note"),
};


USTRUCT(BlueprintType)
struct FInnoCard
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inno")
		int32 Id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inno")
		int32 Age;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inno")
		EInnoColor Color;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inno")
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inno")
		TArray<EInnoResource> Icons;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inno")
		EInnoResource DogmaResource;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inno")
		TArray<FText> Dogmas;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inno")
		TArray<FText> Echoes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inno")
		TArray<FText> Inspire;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inno")
		FText Karma;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inno")
		FText Text;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inno")
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

USTRUCT(BlueprintType)
struct FInnoCardInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 CardId;

	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 Set;

	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 Age;

	// For distinguishing unknown cards with the same back in the same container.
	// The container that holds the widget can modify it.
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 Index;

	FInnoCardInfo() : CardId(0), Set(0), Age(0), Index(INDEX_NONE) {}

	static FInnoCardInfo FromCard(const FInnoCard& Card)
	{
		FInnoCardInfo Result;
		Result.CardId = Card.Id;
		Result.Set = Card.Set;
		Result.Age = Card.Age;
		Result.Index = INDEX_NONE;
		return Result;
	}

	FORCEINLINE bool IsExplicit() const
	{
		return CardId > 0;
	}

	FORCEINLINE bool IsValid() const
	{
		return IsExplicit() || (Age >= 1 && Age <= 10 && Set >= 0 && Set <= 4);
	}

	bool operator==(const FInnoCardInfo& Other) const
	{
		const bool bValid = IsValid();
		const bool bOtherValid = Other.IsValid();

		if (bValid != bOtherValid) return false;
		if (!bValid && !bOtherValid) return true;

		const bool bExplicit = IsExplicit();
		const bool bOtherExplicit = Other.IsExplicit();

		if (bExplicit != bOtherExplicit) return false; // not identical, but still can refer to the same card

		if (bExplicit)
		{
			return CardId == Other.CardId;
		}
		else
		{
			return Set == Other.Set && Age == Other.Age && Index == Other.Index;
		}

	}

	bool operator!=(const FInnoCardInfo& Other) const
	{
		return !(*this == Other);
	}

	FString ToString() const
	{
		if (IsExplicit())
		{
			return FString::Printf(TEXT("[Card%d]"), CardId);
		}
		else
		{
			return FString::Printf(TEXT("{%d}[%d] #%d"), Set, Age, Index);
		}
	}
};

/**
 * 
 */
UCLASS()
class INNOCLIENT_API UInnoCards : public UObject
{
	GENERATED_BODY()

private:
	FString Hash;
	TArray<FInnoCard> Cards;

	bool Load();
	bool Save() const;

	bool bLoaded; // if loaded from disk or updated with json
	inline bool LoadIfNeeded()
	{
		if (!bLoaded)
			return Load();
		return true;
	}

	FInnoCard ConvertCard(const FInnoCardRaw& RawCard, int32 Index) const;

public:
	UInnoCards();

	const static FString CardInfoPath;
	const static FString CardInfoName;
	const static FInnoCard CardNone;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		const FInnoCard& GetCard(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inno")
		void Update(const FString& CardsJson);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inno")
		const TArray<FInnoCard>& GetCards();
	
};

