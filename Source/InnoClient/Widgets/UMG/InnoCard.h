// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Widgets/Slate/SInnoCard.h"
#include "InnoCard.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardEvent, int32, Card);
/**
 * 
 */
UCLASS()
class INNOCLIENT_API UInnoCardWidget : public UWidget
{
	GENERATED_BODY()

protected:
	TSharedPtr<SInnoCard> MyCard;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inno")
		int32 CardId;

	UFUNCTION(BLueprintCallable, Category = "Inno")
		void SetCardId(int32 InCardId);

	UPROPERTY(BlueprintAssignable, Category = "Innovation|Event")
		FCardEvent OnClicked;

	UPROPERTY(BlueprintAssignable, Category = "Innovation|Event")
		FCardEvent OnHovered;

	UPROPERTY(BlueprintAssignable, Category = "Innovation|Event")
		FCardEvent OnUnhovered;

	FReply SlateHandleClicked(FInnoCardInfo CardId);
	void SlateHandleHovered(FInnoCardInfo CardId);
	void SlateHandleUnhovered(FInnoCardInfo CardId);
	
	UInnoCardWidget() : CardId(0) {}
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
	
};
