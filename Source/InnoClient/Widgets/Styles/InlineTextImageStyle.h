#pragma once

#include "Engine/DataAsset.h"
#include "SlateBasics.h"
#include "SlateWidgetStyleContainerBase.h"
#include "SlateWidgetStyle.h"
#include "InlineTextImageStyle.generated.h" 

UCLASS(BlueprintType)
class INNOCLIENT_API UInnoInlineIcons : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Innovation")
		TMap<FName, FInlineTextImageStyle> InlineIcons;
};

// Provides a widget style container to allow us to edit properties in-editor
UCLASS(hidecategories = Object, MinimalAPI)
class UInlineTextImageStyleContainer : public USlateWidgetStyleContainerBase
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Appearance, meta = (ShowOnlyInnerProperties))
		FInlineTextImageStyle Style;

	virtual const FInlineTextImageStyle* const GetStyle() const override
	{
		return &Style;
	}

};
