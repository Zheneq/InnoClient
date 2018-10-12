// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Widgets/Slate/SInnoHand.h"
#include "InnoHand.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardInHandClickedEvent, int32, Card);

/**
 * 
 */
UCLASS()
class INNOCLIENT_API UInnoHand : public UWidget
{
	GENERATED_BODY()

protected:
	TSharedPtr<SInnoHand> MyWidget;

public:
	//~ Begin UWidget Interface
	// virtual void SynchronizeProperties() override;
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
	// TODO : Make base class for all Inno UWidgets and move GM handling there
	TWeakObjectPtr < class AGMInno > GM;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ExposeOnSpawn = true))
		bool bInteractive;

	UFUNCTION(BlueprintCallable, Category = "Innovation")
		void Update(const TArray<int32>& Cards);

	/** Called when the top card is clicked */
	UPROPERTY(BlueprintAssignable, Category = "Innovation|Event")
		FOnCardInHandClickedEvent OnTopCardClicked;

	FReply SlateHandleClicked(int32 CardId);
};
