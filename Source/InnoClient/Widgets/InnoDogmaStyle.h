#pragma once

#include "SlateWidgetStyleContainerBase.h"
#include "SlateWidgetStyle.h"
#include "SlateBasics.h"
#include "InnoDogmaStyle.generated.h" 

// Provides a group of global style settings for our dogmas! 
USTRUCT()
struct FInnoDogmaStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FInnoDogmaStyle();
	~FInnoDogmaStyle();

	// begin FSlateWidgetStyle interface 
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	virtual const FName GetTypeName() const override;
	// end  FSlateWidgetStyle interface 

	// Stores the TypeName for our widget style.
	static const FName TypeName;

	// Allows us to set default values for our various styles. 
	static const FInnoDogmaStyle& GetDefault();

	// Style that define the appearance of all menu buttons. 
	mutable TMap<FString, FSlateBrush> IconBrushes;

	void UpdateBrushes() const;

	UPROPERTY(EditAnywhere, Category = Appearance)
		FVector2D IconSize;

	UPROPERTY(EditDefaultsOnly, Category = Appearance)
		TMap<FString, UTexture2D*> IconTextures;

	// Style for text
	UPROPERTY(EditAnywhere, Category = Appearance)
		FTextBlockStyle TextStyle;
};

// Provides a widget style container to allow us to edit properties in-editor
UCLASS(hidecategories = Object, MinimalAPI)
class UInnoDogmaStyleContainer : public USlateWidgetStyleContainerBase
{
	GENERATED_UCLASS_BODY()

public:
	// This is our actual Style object. 
	UPROPERTY(EditAnywhere, Category = Appearance, meta = (ShowOnlyInnerProperties))
		FInnoDogmaStyle Style;

	// Retrievs the style that this container manages. 
	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast<const struct FSlateWidgetStyle*>(&Style);
	}

};