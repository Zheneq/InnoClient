#pragma once

#include "SlateBasics.h"
#include "SlateWidgetStyleContainerBase.h"
#include "SlateWidgetStyle.h"
#include "InnoCardViewWidgetStyle.generated.h" 

// Style for the widget
USTRUCT()
struct FInnoCardViewStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FInnoCardViewStyle()
		: ScrollSpeed(15.f)
		, ScrollSensitivity(.01f)
		, ChildDominanceThreshold(.75f)
	{};

	// begin FSlateWidgetStyle interface 
	static const FName TypeName;

	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};
	// end  FSlateWidgetStyle interface 

	// Allows us to set default values for our various styles. 
	static const FInnoCardViewStyle& GetDefault()
	{
		static const FInnoCardViewStyle Default;
		return Default;
	};

	// View percentage, 0 for selected widget, +-1 for adjacent ones.
	// Will animate along the curve when scrolling.
	UPROPERTY(EditAnywhere, Category = Appearance)
		FRuntimeFloatCurve Curve;

	// 
	UPROPERTY(EditAnywhere, Category = Appearance)
		float ScrollSpeed;

	// Scroll Sensitivity for touchscreens.
	UPROPERTY(EditAnywhere, Category = Appearance)
		float ScrollSensitivity;

	// How big view percentage a child must have to be considered dominant and to be interactable.
	// Set to 0 if you want instant reaction, and to 1 if you want the widget to be interactable
	// only when animation has finished.
	UPROPERTY(EditAnywhere, Category = Appearance)
		float ChildDominanceThreshold;
};

// Provides a widget style container to allow us to edit properties in-editor
UCLASS(hidecategories = Object, MinimalAPI)
class UFInnoCardViewStyleContainer : public USlateWidgetStyleContainerBase
{
	GENERATED_UCLASS_BODY()

public:
	// This is our actual Style object. 
	UPROPERTY(EditAnywhere, Category = Appearance, meta = (ShowOnlyInnerProperties))
		FInnoCardViewStyle Style;

	// Retrievs the style that this container manages. 
	//virtual const struct FSlateWidgetStyle* const GetStyle() const override
	//{
	//	return static_cast<const struct FSlateWidgetStyle*>(&Style);
	//}
	virtual const FInnoCardViewStyle* const GetStyle() const override
	{
		return &Style;
	}

};
