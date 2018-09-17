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

	FInnoCardViewStyle() :
		ScrollSpeed(15.f),
		ScrollSensitivity(.01f)
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

	// 
	UPROPERTY(EditAnywhere, Category = Appearance)
		FRuntimeFloatCurve Curve;

	// 
	UPROPERTY(EditAnywhere, Category = Appearance)
		float ScrollSpeed;
	// 
	UPROPERTY(EditAnywhere, Category = Appearance)
		float ScrollSensitivity;
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
	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast<const struct FSlateWidgetStyle*>(&Style);
	}

};
