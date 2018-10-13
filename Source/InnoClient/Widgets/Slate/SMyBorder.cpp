// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SMyBorder.h"

int32 SMyBorder::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FSlateBrush* BrushResource = BorderImage.Get();

	const bool bEnabled = ShouldBeEnabled(bParentEnabled);
	const bool bShowDisabledEffect = ShowDisabledEffect.Get();

	if (BrushResource && BrushResource->DrawAs != ESlateBrushDrawType::NoDrawType)
	{
		const ESlateDrawEffect DrawEffects = (bShowDisabledEffect && !bEnabled) ? ESlateDrawEffect::DisabledEffect : ESlateDrawEffect::None;

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			BrushResource,
			DrawEffects,
			BrushResource->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint() * BorderBackgroundColor.Get().GetColor(InWidgetStyle)
		);
	}

	// If we do not show disabled effects, we do not want to pass disabled flag down to hierarchy.
	// If there is a disabled widget below, all its descendats will still show disabled effects.
	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, !bShowDisabledEffect);
}
