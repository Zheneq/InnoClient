#pragma once

#include "SlateBasics.h"
#include "SlateWidgetStyleContainerBase.h"
#include "SlateWidgetStyle.h"
#include "SlateCore/Public/Styling/SlateBrush.h"
#include "SlateEnums.h"
#include "InnoCards.h"
#include "InnoCardWidgetStyle.generated.h" 


// Style for the widget
USTRUCT()
struct FInnoCardStyle : public FSlateWidgetStyle
{
	static const FTextBlockStyle DefaultText;

	GENERATED_USTRUCT_BODY()

		FInnoCardStyle()
		: Width(260)
		, MinHeight(260)
		, BorderThickness(3)
		, BorderPadding(2)
		, ColorAndOpacity(FLinearColor::White)

		, HighlightBrush(FSlateColorBrush(FLinearColor::White))
		, HighlightThickness(4)

		, HeaderHeight(60)
		, IconsFillWidth(3)
		, NameFillWidth(6)
		, AgeFillWidth(1)
		, NameTextStyle(DefaultText)
		, NameHAlign(EHorizontalAlignment::HAlign_Right)
		, NameVAlign(EVerticalAlignment::VAlign_Top)
		, NamePadding(2)
		, NameJustification(ETextJustify::Right)
		, AgeTextStyle(DefaultText)
		, AgePadding(2)
		
		, DogmaWrapTextAt(230)
		, DogmaTextStyle(DefaultText)
		, DogmaKarmaPadding(0)
		, DogmaLineHeightPercentage(1)
		, DogmaJustification(ETextJustify::Left)

		, EchoTextStyle(DefaultText)
		, EchoInspirePadding(0)
		, EchoLineHeightPercentage(1)
		, EchoJustification(ETextJustify::Left)

		, InspireTextStyle(DefaultText)
		, InspireLineHeightPercentage(1)
		, InspireJustification(ETextJustify::Left)

		, KarmaTextStyle(DefaultText)
		, KarmaLineHeightPercentage(1)
		, KarmaJustification(ETextJustify::Left)
	{};

	// begin FSlateWidgetStyle interface 
	static const FName TypeName;

	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};
	// end  FSlateWidgetStyle interface 

	// Allows us to set default values for our various styles. 
	static const FInnoCardStyle& GetDefault()
	{
		static const FInnoCardStyle Default;
		return Default;
	};
	
	UPROPERTY(EditDefaultsOnly, Category = "Card")
		float Width;
	UPROPERTY(EditDefaultsOnly, Category = "Card")
		float MinHeight;
	UPROPERTY(EditDefaultsOnly, Category = "Card", meta = (DisplayName = "Border Style"))
		FButtonStyle ButtonStyle;
	UPROPERTY(EditDefaultsOnly, Category = "Card")
		float BorderThickness;
	UPROPERTY(EditDefaultsOnly, Category = "Card")
		FMargin BorderPadding;
	UPROPERTY(EditDefaultsOnly, Category = "Card")
		FLinearColor ColorAndOpacity;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Card", meta = (DisplayName = "BackgroundColors"))
		TMap<EInnoColor, FLinearColor> BackgroundColors_EDITOR;
	UPROPERTY(EditDefaultsOnly, Category = "Card", meta = (DisplayName = "AgeBackgroundColors"))
		TArray<FLinearColor> AgeBackgroundColors_EDITOR;
#endif // WITH_EDITORONLY_DATA

	UPROPERTY(EditDefaultsOnly, Category = "Card_Highlight")
		FSlateBrush HighlightBrush;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Highlight")
		float HighlightThickness;

	UPROPERTY(EditDefaultsOnly, Category = "Card_Header")
		float HeaderHeight;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Header")
		float IconsFillWidth;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Header")
		float NameFillWidth;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Header")
		float AgeFillWidth;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Header")
		TEnumAsByte<EHorizontalAlignment> NameHAlign;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Header")
		TEnumAsByte<EVerticalAlignment> NameVAlign;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Header")
		FTextBlockStyle NameTextStyle;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Header")
		FMargin NamePadding;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Header")
		TEnumAsByte<ETextJustify::Type> NameJustification;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Header")
		FMargin AgePadding;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Header")
		FTextBlockStyle AgeTextStyle;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Header")
		TMap<EInnoResource, UTexture2D*> Icons;

	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Dogma")
		float DogmaWrapTextAt;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Dogma")
		FTextBlockStyle DogmaTextStyle;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Dogma")
		FMargin DogmaKarmaPadding;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Dogma")
		float DogmaLineHeightPercentage;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Dogma")
		TEnumAsByte<ETextJustify::Type> DogmaJustification;

	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Echo")
		FTextBlockStyle EchoTextStyle;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Echo")
		FMargin EchoInspirePadding;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Echo")
		float EchoLineHeightPercentage;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Echo")
		TEnumAsByte<ETextJustify::Type> EchoJustification;

	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Inspire")
		FTextBlockStyle InspireTextStyle;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Inspire")
		float InspireLineHeightPercentage;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Inspire")
		TEnumAsByte<ETextJustify::Type> InspireJustification;

	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Karma")
		FTextBlockStyle KarmaTextStyle;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Karma")
		float KarmaLineHeightPercentage;
	UPROPERTY(EditDefaultsOnly, Category = "Card_Effects_Karma")
		TEnumAsByte<ETextJustify::Type> KarmaJustification;

	UPROPERTY()
		TMap<EInnoColor, FSlateBrush> BackgroundColorBrushes;
	UPROPERTY()
		TArray<FSlateBrush> AgeBackgroundColorBrushes;

	const FSlateBrush* BackgroundColorBrush(EInnoColor Color) const;
	const FSlateBrush* AgeBackgroundColorBrush(int32 Set) const;
};

// Provides a widget style container to allow us to edit properties in-editor
UCLASS(hidecategories = Object, MinimalAPI)
class UInnoCardStyleContainer : public USlateWidgetStyleContainerBase
{
	GENERATED_UCLASS_BODY()

public:
	// This is our actual Style object. 
	UPROPERTY(EditDefaultsOnly, Category = Appearance, meta = (ShowOnlyInnerProperties))
		FInnoCardStyle Style;

	virtual const FInnoCardStyle* const GetStyle() const override
	{
		return &Style;
	}

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
};
