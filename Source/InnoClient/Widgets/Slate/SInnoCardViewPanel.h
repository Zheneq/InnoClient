#pragma once

#include "SlateBasics.h"

class INNOCLIENT_API SInnoCardViewPanel : public SPanel
{
public:
	/** A Slot that provides layout options for the contents of a scrollable box. */
	class INNOCLIENT_API FSlot : public TSlotBase<FSlot>, public TSupportsContentPaddingMixin<FSlot>
	{
	public:
		FSlot()
			: TSlotBase<FSlot>()
			// Set both vertical and horizontal alignment to fill by default.  During layout, the
			// alignment direction parallel to the scroll direction is assumed to be top, or left
			// since that is how the items are stacked.
			, HAlignment(HAlign_Fill)
			, VAlignment(VAlign_Fill)
		{
		}

		FSlot& HAlign(EHorizontalAlignment InHAlignment)
		{
			HAlignment = InHAlignment;
			return *this;
		}

		FSlot& VAlign(EVerticalAlignment InVAlignment)
		{
			VAlignment = InVAlignment;
			return *this;
		}

		EHorizontalAlignment HAlignment;
		EVerticalAlignment VAlignment;
	};

public:

	SLATE_BEGIN_ARGS(SInnoCardViewPanel):
		_Orientation(EOrientation::Orient_Vertical),
		_bAnimateScroll(true),
		_StyleOverride(nullptr)
	{
		_Visibility = EVisibility::Visible;
	}
	SLATE_SUPPORTS_SLOT(FSlot)
	SLATE_ARGUMENT(EOrientation, Orientation)
	SLATE_ARGUMENT(bool, bAnimateScroll)
	SLATE_ARGUMENT(const struct FInnoCardViewStyle*, StyleOverride)
	SLATE_END_ARGS()

	SInnoCardViewPanel():
		Children(this)
	{
	}

	void Construct(const FArguments& InArgs);
	void Construct(const FArguments& InArgs, const TArray<FSlot*>& InSlots);

public:

	EOrientation GetOrientation()
	{
		return Orientation;
	}

	void SetOrientation(EOrientation InOrientation)
	{
		Orientation = InOrientation;
	}

	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;

	virtual FChildren* GetChildren() override
	{
		return &Children;
	}

	TPanelChildren<FSlot> Children;

protected:
	virtual FVector2D ComputeDesiredSize(float) const override;

public:
	/** @return a new slot. Slots contain children for SInnoCardViewWidget */
	static FSlot& Slot();

	/** Adds a slot to SInnoCardViewWidget */
	FSlot& AddSlot();

	/** Removes a slot at the specified location */
	void RemoveSlot(const TSharedRef<SWidget>& WidgetToRemove);

	/** Removes all children from the box */
	void ClearChildren();

	/** */
	void SetIsSelected(bool bNewActive);

	/** */
	void ToggleIsSelected();

	/**  */
	bool bAnimateScroll;

	/**  */
	void SetStyle(const struct FInnoCardViewStyle* StyleOverride)
	{
		if (StyleOverride)
		{
			Style = StyleOverride;
		}
		else
		{
			Style = DefaultStyle;
		}
	}

public:

	// SWidget interface
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual bool ComputeVolatility() const override;
	virtual FReply OnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override;
	virtual void OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;
	// End of SWidget interface

protected:
	/** The scrolling and stacking orientation. */
	EOrientation Orientation;

	/** Whether we highlight item at SelectedIndex or just spread all the items evenly */
	bool bIsSelected;

	/** Highlighted item (or the item we are in progress of scrolling to) */
	int32 SelectedIndex;

	/** Actual highlight */
	float CurrentScroll;

	/** Are we in progress of scrolling? */
	bool bIsScrolling;

	/** Did the user start an interaction in this list? */
	TOptional<int32> bFingerOwningTouchInteraction;

	/** The current deviation we've accumulated on scroll, once it passes the trigger amount, we're going to begin scrolling. */
	float PendingScrollTriggerAmount;

	/** Input that is registered but not applied yet (below the sensitivity threshold) */
	float CachedScroll;

	bool bTouchPanningCapture : 1;

	bool bHasNotJustTouched;
	bool bProbablyLostFocus;

	mutable TArray<float> ChildEndsAt;

	void UpdateChildrenInteraction() const;

	/** Gets the component of a vector in the direction of scrolling based on the Orientation property. */
	FORCEINLINE float GetScrollComponentFromVector(FVector2D Vector) const
	{
		return Orientation == Orient_Vertical ? Vector.Y : Vector.X;
	}

	void SetSelectedIndexByPosition(float Position)
	{
		for (int32 i = 0; i < Children.Num(); ++i)
		{
			if (ChildEndsAt[i] >= Position)
			{
				SetSelectedIndex(i);
				break;
			}
		}
	}

public:
	TAttribute<const FText*> Text;

	TSharedPtr<class SCardViewPanel> Panel;

	// Style in use
	const struct FInnoCardViewStyle* Style;

	// Style to fall back to
	const struct FInnoCardViewStyle* DefaultStyle;

	void SetSelectedIndex(int32 Index);
};
