
#include "InnoCardWidgetStyle.h" 
#include "InnoClient.h" 

const FName FInnoCardStyle::TypeName = TEXT("FInnoCardStyle");

const FTextBlockStyle FInnoCardStyle::DefaultText = FTextBlockStyle()
	.SetFont(FCoreStyle::GetDefaultFontStyle("Regular", 14))
	.SetColorAndOpacity(FLinearColor::Black);

const FSlateBrush* FInnoCardStyle::BackgroundColorBrush(EInnoColor Color) const
{
	if (BackgroundColorBrushes.Contains(Color))
	{
		return &BackgroundColorBrushes[Color];
	}

	return nullptr;
}

const FSlateBrush* FInnoCardStyle::AgeBackgroundColorBrush(int32 Set) const
{
	if (AgeBackgroundColorBrushes.IsValidIndex(Set))
	{
		return &AgeBackgroundColorBrushes[Set];
	}

	return nullptr;
}

UInnoCardStyleContainer::UInnoCardStyleContainer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITOR
void UInnoCardStyleContainer::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	Style.BackgroundColorBrushes.Empty(Style.BackgroundColors_EDITOR.Num());
	for (const auto& c : Style.BackgroundColors_EDITOR)
	{
		Style.BackgroundColorBrushes.Add(c.Key, FSlateColorBrush(c.Value));
	}

	Style.AgeBackgroundColorBrushes.Empty(Style.AgeBackgroundColors_EDITOR.Num());
	for (int32 i = 0; i < Style.AgeBackgroundColors_EDITOR.Num(); ++i)
	{
		Style.AgeBackgroundColorBrushes.Add(FSlateColorBrush(Style.AgeBackgroundColors_EDITOR[i]));
	}
}
#endif // WITH_EDITOR
