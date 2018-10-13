#pragma once

#include "SlateBasics.h"

class INNOCLIENT_API SMyBorder : public SBorder
{

public:

	// SWidget interface
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	// End of SWidget interface
};
