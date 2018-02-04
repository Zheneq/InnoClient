#include "InnoWidgetStyles.h"
#include "InnoClient.h"
#include "SlateGameResources.h" 

TSharedPtr<FSlateStyleSet> FInnoWidgetStyles::StyleInstance = NULL;

void FInnoWidgetStyles::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FInnoWidgetStyles::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FInnoWidgetStyles::GetStyleSetName()
{
	static FName StyleSetName(TEXT("InnoStyles"));
	return StyleSetName;
}

TSharedRef<FSlateStyleSet> FInnoWidgetStyles::Create()
{
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(FInnoWidgetStyles::GetStyleSetName(), "/Game/UI/Styles", "/Game/UI/Styles");
	return StyleRef;
}

const ISlateStyle& FInnoWidgetStyles::Get()
{
	return *StyleInstance;
}