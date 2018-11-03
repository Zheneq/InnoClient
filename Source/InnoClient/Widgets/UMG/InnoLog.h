// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Widgets/Slate/SInnoGameLog.h"
#include "InnoLog.generated.h"

/**
 * 
 */
UCLASS()
class INNOCLIENT_API UInnoLog : public UWidget
{
	GENERATED_BODY()

protected:
	TSharedPtr<SInnoGameLog> MyWidget;

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

public:
	UFUNCTION(BlueprintCallable, Category = "Innovation")
		void AddMessage(const FText& Message, bool bReplaceLastMessage);

	UFUNCTION(BlueprintCallable, Category = "Innovation")
		void Clear();
};
