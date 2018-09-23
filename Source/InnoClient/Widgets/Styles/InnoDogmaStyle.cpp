#include "InnoDogmaStyle.h" 
#include "InnoClient.h" 


FInnoDogmaStyle::FInnoDogmaStyle()
{
	UE_LOG(LogTemp, Log, TEXT("FInnoDogmaStyle"));
	for (const auto& T : IconTextures)
	{
		UE_LOG(LogTemp, Log, TEXT("FInnoDogmaStyle-%d"), T.Value);
	}
	UE_LOG(LogTemp, Log, TEXT("-FInnoDogmaStyle"));
}

FInnoDogmaStyle::~FInnoDogmaStyle()
{

}

void FInnoDogmaStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	for (const auto& Brush : IconBrushes)
	{
		OutBrushes.Add(&Brush.Value);
	}
}

const FName FInnoDogmaStyle::TypeName = TEXT("FInnoDogmaStyle");

const FName FInnoDogmaStyle::GetTypeName() const
{
	return TypeName;
}

const FInnoDogmaStyle& FInnoDogmaStyle::GetDefault()
{
	static const FInnoDogmaStyle Default;
	return Default;
}

void FInnoDogmaStyle::UpdateBrushes() const
{
	// Always make full update in the editor as the asset can be modified
#ifndef WITH_EDITOR
	if (IconBrushes.Num() == 0 && IconTextures.Num() != 0)
#endif
	{
		// Create brushes
		for (const auto& Texture : IconTextures)
		{
			if (Texture.Value)
			{
				FSlateBrush Brush;
				Brush.SetResourceObject(Texture.Value);
				int32 Width  = (IconSize.X > 0) ? IconSize.X : Texture.Value->GetSizeX();
				int32 Height = (IconSize.Y > 0) ? IconSize.Y : Texture.Value->GetSizeY();
				Brush.ImageSize = FVector2D(Width, Height);
				IconBrushes.Add(Texture.Key, Brush);
			}
			else
			{
				IconBrushes.Add(Texture.Key, FSlateNoResource());
			}
		}
	}
#ifndef WITH_EDITOR
	else
	{
		// Update size only, textures are not supposed to change in runtime
		if (IconSize.X > 0 && IconSize.Y > 0)
		{
			for (auto& Brush : IconBrushes)
			{
				Brush.Value.ImageSize = IconSize;
			}
		}
	}
#endif
}

UInnoDogmaStyleContainer::UInnoDogmaStyleContainer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}