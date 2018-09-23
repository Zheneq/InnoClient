// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Widgets/SWidget.h"
#include "Widgets/Slate/SInnoCardViewPanel.h"
#include "Components/PanelWidget.h"
#include "InnoCardView.generated.h"

class UInnoCardViewSlot;

/**
 * 
 */
UCLASS()
class INNOCLIENT_API UInnoCardView : public UPanelWidget
{
	GENERATED_BODY()
	
public:
	UInnoCardView(const FObjectInitializer& ObjectInitializer);

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Widget")
		UInnoCardViewSlot* AddChildToCardView(UWidget* Content);

	UPROPERTY(EditAnywhere, Category = "Widget")
		TEnumAsByte<EOrientation> Orientation;

	UPROPERTY(EditAnywhere, Category = "Widget")
		int32 DefaultSelection;

	UPROPERTY(EditAnywhere, Category = "Widget")
		bool bDefaultSelected;

	UPROPERTY(EditAnywhere, Category = "Widget")
		bool bAnimateScroll;

	UPROPERTY(EditAnywhere, Category = "Widget")
		class USlateWidgetStyleAsset* StyleOverride;

	UFUNCTION(BlueprintCallable, Category = "Widget")
		void Select(int32 Index);

#if WITH_EDITOR
	// UObject Interface
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	// End UObject Interface

	// UWidget interface
	virtual const FText GetPaletteCategory() override;
	// End UWidget interface
#endif

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:

	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;
	// End UPanelWidget

protected:
	TSharedPtr<SInnoCardViewPanel> MyInnoCardViewPanel;

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface
};
