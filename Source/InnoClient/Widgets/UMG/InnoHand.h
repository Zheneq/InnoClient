// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Widgets/Slate/SInnoHand.h"
#include "InnoHand.generated.h"

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

public:

	// Inno commands
	void Update(int32 PlayerId, const struct FInnoPlayerInfo& PlayerInfo);
	void Play(int32 PlayId, int32 DrawDeck, const TArray<int32>& Achs, const TArray<int32>& Decree, const TArray<EInnoColor>& Inspire, const TArray<EInnoColor>& Endorse);
};
