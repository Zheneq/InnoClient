#pragma once

#include "SlateBasics.h"
#include "SlateWidgetStyleContainerBase.h"
#include "SlateWidgetStyle.h"
//#include "SlateCore/Public/Styling/SlateBrush.h"
//#include "SlateEnums.h"
//#include "InnoCards.h"
#include "InnoLogStyle.generated.h" 

// Style for Inno Log widget
USTRUCT()
struct FInnoLogStyle : public FSlateWidgetStyle
{
	static const FTextBlockStyle DefaultText;

	GENERATED_USTRUCT_BODY()

		FInnoLogStyle()
		: WrapTextAt(0)
		, AutoWrapText(false)
		, WrappingPolicy(ETextWrappingPolicy::DefaultWrapping)
		, TextStyle()
		, ContentPadding(FMargin(3.f, 3.f, 3.f, 10.f))
		, MessagePadding(10)
		, Justification(ETextJustify::Left)
		, LineHeightPercentage(1)
		, InlineCardShift(-5.5)
		, MaxMessages(20)
	{};

	// begin FSlateWidgetStyle interface 
	static const FName TypeName;

	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};
	// end  FSlateWidgetStyle interface 

	// Allows us to set default values for our various styles. 
	static const FInnoLogStyle& GetDefault()
	{
		static const FInnoLogStyle Default;
		return Default;
	};

	UPROPERTY(EditDefaultsOnly, Category = "Log")
		float WrapTextAt;
	UPROPERTY(EditDefaultsOnly, Category = "Log")
		bool AutoWrapText;
	UPROPERTY(EditDefaultsOnly, Category = "Log")
		ETextWrappingPolicy WrappingPolicy;
	UPROPERTY(EditDefaultsOnly, Category = "Log")
		FTextBlockStyle TextStyle;
	UPROPERTY(EditDefaultsOnly, Category = "Log")
		FMargin ContentPadding;
	UPROPERTY(EditDefaultsOnly, Category = "Log")
		FMargin MessagePadding;
	UPROPERTY(EditDefaultsOnly, Category = "Log")
		TEnumAsByte<ETextJustify::Type> Justification;
	UPROPERTY(EditDefaultsOnly, Category = "Log")
		float LineHeightPercentage;
	UPROPERTY(EditDefaultsOnly, Category = "Log")
		float InlineCardShift;
	UPROPERTY(EditDefaultsOnly, Category = "Log")
		int32 MaxMessages;
};

// Provides a widget style container to allow us to edit properties in-editor
UCLASS(hidecategories = Object, MinimalAPI)
class UInnoLogStyleContainer : public USlateWidgetStyleContainerBase
{
	GENERATED_UCLASS_BODY()

public:
	// This is our actual Style object. 
	UPROPERTY(EditDefaultsOnly, Category = Appearance, meta = (ShowOnlyInnerProperties))
		FInnoLogStyle Style;

	virtual const FInnoLogStyle* const GetStyle() const override
	{
		return &Style;
	}
};
