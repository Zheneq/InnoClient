// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Widgets/Slate/SInnoCardList.h"
#include "InnoCardList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardClickedEvent, int32, Card);

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
		bool bInteractive;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ExposeOnSpawn = true))
		FName Tag;

	UFUNCTION(BlueprintCallable, Category = "Innovation")
		void Update(const TArray<int32>& Cards);

	UFUNCTION(BlueprintCallable, Category = "Innovation")
		void SetIsInteractive(bool bNewInteractive);

	/** Called when the top card is clicked */
	UPROPERTY(BlueprintAssignable, Category = "Innovation|Event")
		FOnCardClickedEvent OnCardClicked;

	FReply SlateHandleClicked(int32 CardId);
};
