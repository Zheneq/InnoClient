// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"
#include "Layout/Margin.h"
#include "Widgets/Slate/SInnoCardViewPanel.h"
#include "Components/PanelSlot.h"
#include "Components/SlateWrapperTypes.h"

#include "InnoCardViewSlot.generated.h"

UCLASS()
class INNOCLIENT_API UInnoCardViewSlot : public UPanelSlot
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|Inno Card View Slot")
		TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|Inno Card View Slot")
		TEnumAsByte<EVerticalAlignment> VerticalAlignment;

public:
	UFUNCTION(BlueprintCallable, Category = "Layout|Inno Card View Slot")
		void SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment);

	UFUNCTION(BlueprintCallable, Category = "Layout|Inno Card View Slot")
		void SetVerticalAlignment(EVerticalAlignment InVerticalAlignment);

	void BuildSlot(TSharedRef<SInnoCardViewPanel> InnoCardViewPanel);

	// UPanelSlot interface
	virtual void SynchronizeProperties() override;
	// End of UPanelSlot interface

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	SInnoCardViewPanel::FSlot* Slot;
};
