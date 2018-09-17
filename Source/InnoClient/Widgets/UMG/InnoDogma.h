// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Widgets/Slate/SInnoDogmaWidget.h"
#include "InnoDogma.generated.h"

/**
 * 
 */
UCLASS()
class INNOCLIENT_API UInnoDogma : public UWidget
{
	GENERATED_BODY()

protected:
	TSharedPtr<SInnoDogmaWidget> MyDogma;
	
public:

	/** Color and opacity multiplier of content in the border */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Content")
		FText Text;

	UFUNCTION(BlueprintCallable, Category = "Inno")
		void SetText(FText InText);

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
	//virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface

	//virtual const FText GetPaletteCategory() override;
#endif
	
};
