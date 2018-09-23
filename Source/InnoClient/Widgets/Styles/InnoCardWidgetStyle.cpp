
#include "InnoCardWidgetStyle.h" 
#include "InnoClient.h" 

const FName FInnoCardStyle::TypeName = TEXT("FInnoCardStyle");

const FTextBlockStyle FInnoCardStyle::DefaultText = FTextBlockStyle()
	.SetFont(FCoreStyle::GetDefaultFontStyle("Regular", 14))
	.SetColorAndOpacity(FLinearColor::Black);

UInnoCardStyleContainer::UInnoCardStyleContainer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}
