// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Widgets/Slate/SInnoCardList.h"
#include "InnoCardList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardClickedEvent, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionValidityEvent, bool, bIsNowValid);

/**
 * 
 */
UCLASS()
class INNOCLIENT_API UInnoCardList : public UWidget
{
	GENERATED_BODY()

protected:
	TSharedPtr<SInnoCardList> MyWidget;

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

#if WITH_EDITOR
	//~ Begin UObject Interface
	// virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface

	virtual const FText GetPaletteCategory() override;
#endif

protected:
	TWeakObjectPtr < class AGMInno > GM;


public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ExposeOnSpawn = true))
		int32 SelectMin;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ExposeOnSpawn = true))
		int32 SelectMax;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ExposeOnSpawn = true))
		FName Tag;

	UFUNCTION(BlueprintCallable, Category = "Innovation")
		void Update(const TArray<struct FInnoCardInfo>& Cards, int32 NewSelectMin = 0, int32 NewSelectMax = 0);

	UFUNCTION(BlueprintCallable, Category = "Innovation")
		void UpdateExplicit(const TArray<int32>& Cards, int32 NewSelectMin = 0, int32 NewSelectMax = 0);

	UFUNCTION(BlueprintCallable, Category = "Innovation")
		void UpdateOptions(int32 NewSelectMin, int32 NewSelectMax);

	UFUNCTION(BlueprintCallable, Category = "Innovation")
		TArray<int32> GetSelectedIndices();

	/** Called when the top card is clicked */
	UPROPERTY(BlueprintAssignable, Category = "Innovation|Event")
		FOnCardClickedEvent OnCardClicked;

	/** Called when the top card is clicked */
	UPROPERTY(BlueprintAssignable, Category = "Innovation|Event")
		FOnCardClickedEvent OnCardToggled;

	/** Called when the top card is clicked */
	UPROPERTY(BlueprintAssignable, Category = "Innovation|Event")
		FOnSelectionValidityEvent OnSelectionValidityChanged;

	FReply SlateHandleClicked(struct FInnoCardInfo CardId);
	FReply SlateHandleSelection(struct FInnoCardInfo CardId);
	void SlateHandleValidity(bool bIsNowValid);
};
