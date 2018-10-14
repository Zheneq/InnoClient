// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Widgets/Slate/SInnoStack.h"
#include "InnoStack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTopCardClickedEvent);

/**
 * 
 */
UCLASS()
class INNOCLIENT_API UInnoStack : public UWidget
{
	GENERATED_BODY()

protected:
	TSharedPtr<SInnoStack> MyWidget;

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
		bool bIsLocalPlayer;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ExposeOnSpawn = true))
		bool bHideText;

	UFUNCTION(BlueprintCallable, Category = "Innovation")
		void Update(const TArray<int32>& Cards, EInnoSplay Splay);

	UFUNCTION(BlueprintCallable, Category = "Innovation")
		void UpdateSplay(EInnoSplay Splay);

	UFUNCTION(BlueprintCallable, Category = "Innovation")
		void SetIsLocalPlayer(bool bNewIsLocalPlayer);

	UFUNCTION(BlueprintCallable, Category = "Innovation")
		void SetHideText(bool bNewHideText);

	/** Called when the top card is clicked */
	UPROPERTY(BlueprintAssignable, Category = "Innovation|Event")
		FOnTopCardClickedEvent OnTopCardClicked;

	FReply SlateHandleClicked(int32 CardId);
};
