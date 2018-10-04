#include "InnoWidgetStyles.h"
#include "InlineTextImageStyle.h"
#include "Engine/DataAsset.h"
#include "EngineUtils.h"
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

const ISlateStyle& FInnoWidgetStyles::Get()
{
	return *StyleInstance;
}

TSharedRef<FSlateStyleSet> FInnoWidgetStyles::Create()
{
	TSharedRef<FSlateStyleSet> Style = FSlateGameResources::New(FInnoWidgetStyles::GetStyleSetName(), "/Game/UI/Styles", "/Game/UI/Styles");
	// StyleRef->SetContentRoot( FPaths::EngineContentDir() / TEXT("Slate") );

	UInnoInlineIcons* Asset = LoadObject<UInnoInlineIcons>(nullptr, TEXT("/Game/UI/Styles/InlineIcons.InlineIcons"));
	if (Asset)
	{
		for (const auto& S : Asset->InlineIcons)
		{
			Style->Set(S.Key, S.Value);
		}
	}

	/*
	TArray<UObject*> Icons;
	EngineUtils::FindOrLoadAssetsByPath(TEXT("/Game/Images/Res/"), Icons, EngineUtils::ATL_Regular);
	for (const UObject* Icon : Icons)
	{
		if (Icon->IsA<UTexture2D>())
		{
			// make brush
		}
	}
	*/

	return Style;
}
