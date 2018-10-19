// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SInnoCardViewPanel.h"
#include "Widgets/Styles/InnoCardViewWidgetStyle.h"
#include "Widgets/Styles/InnoWidgetStyles.h"
#include "InnoClient.h"
#include "Engine.h"


void SInnoCardViewPanel::Construct(const FArguments& InArgs, const TArray<SInnoCardViewPanel::FSlot*>& InSlots)
{
	Children.Reserve(InSlots.Num());
	for (int32 SlotIndex = 0; SlotIndex < InSlots.Num(); ++SlotIndex)
	{
		Children.Add(InSlots[SlotIndex]);
	}
	Construct(InArgs);
	UpdateChildrenInteraction();
}

void SInnoCardViewPanel::Construct(const FArguments& InArgs)
{
	DefaultStyle = &FInnoWidgetStyles::Get().GetWidgetStyle<FInnoCardViewStyle>("Style_InnoCardView");

	Orientation = InArgs._Orientation;
	bAnimateScroll = InArgs._bAnimateScroll;
	SetStyle(InArgs._StyleOverride);

	SelectedIndex = 0;
	CurrentScroll = 0.f;
	bIsScrolling = false;
	bIsSelected = false;
	bTouchPanningCapture = false;
	bProbablyLostFocus = false;
}


void SInnoCardViewPanel::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	const FRichCurve& Curve = *Style->Curve.GetRichCurveConst();

	int32 VisibleChildrenNum = 0;

	const float AllottedSpace = Orientation == Orient_Vertical ? AllottedGeometry.GetLocalSize().Y : AllottedGeometry.GetLocalSize().X;
	// Compute the space required
	float TotalSpace = 0;
	float SelectedTotalFactor = 0;
	float SelectedTotalSpace = 0;
	TArray<float> Space;

	for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
	{
		const FSlot& CurChild = Children[ChildIndex];
		const float ChildFactor = bIsSelected ? Curve.Eval(ChildIndex - CurrentScroll) : 0.f;
		float ChildSize = 0;
		if (CurChild.GetWidget()->GetVisibility() != EVisibility::Collapsed)
		{
			++VisibleChildrenNum;

			// All widgets contribute their margin to the fixed space requirement
			ChildSize = ((Orientation == Orient_Vertical) ? CurChild.SlotPadding.Get().GetTotalSpaceAlong<Orient_Vertical>() : CurChild.SlotPadding.Get().GetTotalSpaceAlong<Orient_Horizontal>());

			FVector2D ChildDesiredSize = CurChild.GetWidget()->GetDesiredSize();

			// Auto-sized children contribute their desired size to the fixed space requirement
			ChildSize += (Orientation == Orient_Vertical)
				? ChildDesiredSize.Y
				: ChildDesiredSize.X;

			TotalSpace += ChildSize;
			SelectedTotalSpace += ChildSize * ChildFactor;
			SelectedTotalFactor += ChildFactor;
		}
		Space.Add(ChildSize);
	}

	UpdateChildrenInteraction();

	if (!VisibleChildrenNum)
	{
		return;
	}

	ChildEndsAt.Empty(Children.Num());

	if (/*bIsSelected ||*/ TotalSpace > AllottedSpace)
	{
		// Minimal space a widget can occupy (size of the widget that is far enough from the selecfed one)
		const float MinimalSpace = (AllottedSpace - SelectedTotalSpace) / (VisibleChildrenNum - SelectedTotalFactor);


		float CurChildOffset = 0;

		for (int32 SlotIndex = 0; SlotIndex < Children.Num(); ++SlotIndex)
		{
			const FSlot& ThisSlot = Children[SlotIndex];
			const EVisibility ChildVisibility = ThisSlot.GetWidget()->GetVisibility();
			const float ChildFactor = bIsSelected ? Curve.Eval(SlotIndex - CurrentScroll) : 0.f;

			if (ChildVisibility != EVisibility::Collapsed)
			{
				const FMargin& ThisPadding = ThisSlot.SlotPadding.Get();
				const FVector2D& WidgetDesiredSize = ThisSlot.GetWidget()->GetDesiredSize();
				const float ThisSlotDesiredSize = ((Orientation == Orient_Vertical) ? WidgetDesiredSize.Y + ThisPadding.GetTotalSpaceAlong<Orient_Vertical>() : WidgetDesiredSize.X + ThisPadding.GetTotalSpaceAlong<Orient_Horizontal>());
				const float ThisSlotSize = MinimalSpace + ChildFactor * (ThisSlotDesiredSize - MinimalSpace);
				if (Orientation == Orient_Vertical)
				{
					// Figure out the size and local position of the child within the slot.  There is no vertical alignment, because 
					// it does not make sense in a panel where items are stacked vertically end-to-end.
					AlignmentArrangeResult XAlignmentResult = AlignChild<Orient_Horizontal>(AllottedGeometry.GetLocalSize().X, ThisSlot, ThisPadding);

					ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(ThisSlot.GetWidget(), FVector2D(XAlignmentResult.Offset, CurChildOffset + ThisPadding.Top), FVector2D(XAlignmentResult.Size, WidgetDesiredSize.Y)));
				}
				else
				{
					// Figure out the size and local position of the child within the slot.  There is no horizontal alignment, because
					// it doesn't make sense in a panel where items are stacked horizontally end-to-end.
					AlignmentArrangeResult YAlignmentResult = AlignChild<Orient_Vertical>(AllottedGeometry.GetLocalSize().Y, ThisSlot, ThisPadding);

					ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(ThisSlot.GetWidget(), FVector2D(CurChildOffset + ThisPadding.Left, YAlignmentResult.Offset), FVector2D(WidgetDesiredSize.X, YAlignmentResult.Size)));
				}
				CurChildOffset += ThisSlotSize;
				ChildEndsAt.Add(CurChildOffset);
			}
		}
	}
	else
		// Just put the children one after another
	{
		float CurChildOffset = 0;

		for (int32 SlotIndex = 0; SlotIndex < Children.Num(); ++SlotIndex)
		{
			const FSlot& ThisSlot = Children[SlotIndex];
			const EVisibility ChildVisibility = ThisSlot.GetWidget()->GetVisibility();

			if (ChildVisibility != EVisibility::Collapsed)
			{
				const FMargin& ThisPadding = ThisSlot.SlotPadding.Get();
				const FVector2D& WidgetDesiredSize = ThisSlot.GetWidget()->GetDesiredSize();
				const float ThisSlotDesiredSize = ((Orientation == Orient_Vertical) ? WidgetDesiredSize.Y + ThisPadding.GetTotalSpaceAlong<Orient_Vertical>() : WidgetDesiredSize.X + ThisPadding.GetTotalSpaceAlong<Orient_Horizontal>());
				if (Orientation == Orient_Vertical)
				{
					// Figure out the size and local position of the child within the slot.  There is no vertical alignment, because 
					// it does not make sense in a panel where items are stacked vertically end-to-end.
					AlignmentArrangeResult XAlignmentResult = AlignChild<Orient_Horizontal>(AllottedGeometry.GetLocalSize().X, ThisSlot, ThisPadding);

					ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(ThisSlot.GetWidget(), FVector2D(XAlignmentResult.Offset, CurChildOffset + ThisPadding.Top), FVector2D(XAlignmentResult.Size, WidgetDesiredSize.Y)));
				}
				else
				{
					// Figure out the size and local position of the child within the slot.  There is no horizontal alignment, because
					// it doesn't make sense in a panel where items are stacked horizontally end-to-end.
					AlignmentArrangeResult YAlignmentResult = AlignChild<Orient_Vertical>(AllottedGeometry.GetLocalSize().Y, ThisSlot, ThisPadding);

					ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(ThisSlot.GetWidget(), FVector2D(CurChildOffset + ThisPadding.Left, YAlignmentResult.Offset), FVector2D(WidgetDesiredSize.X, YAlignmentResult.Size)));
				}
				CurChildOffset += ThisSlotDesiredSize;
				ChildEndsAt.Add(CurChildOffset);
			}
		}
	}
}

int32 SInnoCardViewPanel::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	FArrangedChildren ArrangedChildren(EVisibility::Visible);
	{
		// The panel has no visualization of its own; it just visualizes its children.
		ArrangeChildren(AllottedGeometry, ArrangedChildren);
	}

	// Because we paint multiple children, we must track the maximum layer id that they produced in case one of our parents
	// wants to an overlay for all of its contents.
	int32 MaxLayerId = LayerId;

	const FPaintArgs NewArgs = Args.WithNewParent(this);
	const bool bChildrenEnabled = ShouldBeEnabled(bParentEnabled);

	// We want to paint our children on separate layers so that they overlap properly
	for (int32 ChildIndex = 0; ChildIndex < ArrangedChildren.Num(); ++ChildIndex)
	{
		// We don't increment the first layer.
		if (ChildIndex > 0)
		{
			MaxLayerId++;
		}

		FArrangedWidget& CurWidget = ArrangedChildren[ChildIndex];

		const int32 CurWidgetsMaxLayerId =
			CurWidget.Widget->Paint(
				NewArgs,
				CurWidget.Geometry,
				MyCullingRect,
				OutDrawElements,
				MaxLayerId,
				InWidgetStyle,
				bChildrenEnabled);

		MaxLayerId = FMath::Max(MaxLayerId, CurWidgetsMaxLayerId);
	}

	return MaxLayerId;
}

FVector2D SInnoCardViewPanel::ComputeDesiredSize(float) const
{
	FVector2D ThisDesiredSize = FVector2D::ZeroVector;
	for (int32 SlotIndex = 0; SlotIndex < Children.Num(); ++SlotIndex)
	{
		const FSlot& ThisSlot = Children[SlotIndex];
		if (ThisSlot.GetWidget()->GetVisibility() != EVisibility::Collapsed)
		{
			const FVector2D ChildDesiredSize = ThisSlot.GetWidget()->GetDesiredSize();
			if (Orientation == Orient_Vertical)
			{
				ThisDesiredSize.X = FMath::Max(ChildDesiredSize.X, ThisDesiredSize.X);
				ThisDesiredSize.Y += ChildDesiredSize.Y + ThisSlot.SlotPadding.Get().GetTotalSpaceAlong<Orient_Vertical>();
			}
			else
			{
				ThisDesiredSize.X += ChildDesiredSize.X + ThisSlot.SlotPadding.Get().GetTotalSpaceAlong<Orient_Horizontal>();
				ThisDesiredSize.Y = FMath::Max(ChildDesiredSize.Y, ThisDesiredSize.Y);
			}
		}
	}

	return ThisDesiredSize;
}

SInnoCardViewPanel::FSlot& SInnoCardViewPanel::Slot()
{
	return *(new FSlot());
}

/** Adds a slot to SInnoCardViewPanel */
SInnoCardViewPanel::FSlot& SInnoCardViewPanel::AddSlot()
{
	FSlot& NewSlot = *new FSlot();
	Children.Add(&NewSlot);
	return NewSlot;
}

/** Removes a slot at the specified location */
void SInnoCardViewPanel::RemoveSlot(const TSharedRef<SWidget>& WidgetToRemove)
{
	for (int32 SlotIndex = 0; SlotIndex < Children.Num(); ++SlotIndex)
	{
		if (Children[SlotIndex].GetWidget() == WidgetToRemove)
		{
			Children.RemoveAt(SlotIndex);
			return;
		}
	}
}

void SInnoCardViewPanel::ClearChildren()
{
	Children.Empty();
}

void SInnoCardViewPanel::SetSelectedIndex(int32 Index)
{
	SelectedIndex = FMath::Clamp(Index, 0, Children.Num() - 1);
	UpdateChildrenInteraction();
}

void SInnoCardViewPanel::SetChildrenEnabled(bool bNewChildrenEnabled)
{
	for (int32 i = 0; i < Children.Num(); ++i)
	{
		const auto Child = Children.GetChildAt(i);
		Child->SetEnabled(bNewChildrenEnabled);
	}
}

void SInnoCardViewPanel::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	const bool bWasScrolling = bIsScrolling;
	bIsScrolling = !FMath::IsNearlyEqual(SelectedIndex, CurrentScroll, 0.001f);
	bCanClick = FMath::Abs(SelectedIndex - CurrentScroll) < Style->ChildDominanceThreshold;
	CurrentScroll = (bAnimateScroll)
		? FMath::FInterpTo(CurrentScroll, SelectedIndex, InDeltaTime, Style->ScrollSpeed)
		: SelectedIndex;

	if (bWasScrolling && !bIsScrolling)
	{
		UpdateChildrenInteraction();
		Invalidate(EInvalidateWidget::Layout);
	}

	if (bProbablyLostFocus)
	{
		bProbablyLostFocus = false;
		SetIsSelected(false);
	}
}

bool SInnoCardViewPanel::ComputeVolatility() const
{
	return bIsScrolling;
}

void SInnoCardViewPanel::SetIsSelected(bool bNewSelected)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Yellow, FString::Printf(TEXT("SetIsSelected, was %d, now %d"), bIsSelected, bNewSelected));

	if (bNewSelected == bIsSelected) return;

	bIsSelected = bNewSelected;

	UpdateChildrenInteraction();
	// Invalidate(EInvalidateWidget::Layout);
}

void SInnoCardViewPanel::ToggleIsSelected()
{
	SetIsSelected(!bIsSelected);
}

void SInnoCardViewPanel::UpdateChildrenInteraction() const
{
	for (int32 SlotIndex = 0; SlotIndex < Children.Num(); ++SlotIndex)
	{
		const TSharedRef<SWidget> Widget = Children[SlotIndex].GetWidget();
		const EVisibility Vis = Widget->GetVisibility();
		if (Vis.IsVisible())
		{
			Widget->SetVisibility(bIsSelected && bCanClick && SelectedIndex == SlotIndex ? EVisibility::Visible : EVisibility::HitTestInvisible);
		}
	}
}

FReply SInnoCardViewPanel::OnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsTouchEvent() && !bFingerOwningTouchInteraction.IsSet())
	{
		// We have started a new interaction; track how far the user has moved since they put their finger down.
		PendingScrollTriggerAmount = 0;
		// Reset cached scroll
		CachedScroll = 0;
		// Someone put their finger down in this list, so they probably want to drag the list.
		bFingerOwningTouchInteraction = MouseEvent.GetPointerIndex();

		bHasNotJustTouched = false;
		bProbablyLostFocus = false;

		Invalidate(EInvalidateWidget::Layout);
	}
	return FReply::Unhandled();
}

FReply SInnoCardViewPanel::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsTouchEvent())
	{
		return FReply::Handled().CaptureMouse(AsShared());
	}

	return FReply::Unhandled();
}

FReply SInnoCardViewPanel::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const float ScrollByAmountScreen = GetScrollComponentFromVector(MouseEvent.GetCursorDelta());
	const float ScrollByAmountLocal = ScrollByAmountScreen / MyGeometry.Scale;

	if (MouseEvent.IsTouchEvent())
	{
		if (bIsSelected)
		{
			FReply Reply = FReply::Unhandled();

			if (!bTouchPanningCapture)
			{
				if (bFingerOwningTouchInteraction.IsSet() /* && !HasMouseCapture() */)
				{
					PendingScrollTriggerAmount += ScrollByAmountScreen;

					if (FMath::Abs(PendingScrollTriggerAmount) > FSlateApplication::Get().GetDragTriggerDistance())
					{
						bTouchPanningCapture = true;

						// The user has moved the list some amount; they are probably
						// trying to scroll. From now on, the list assumes the user is scrolling
						// until they lift their finger.
						Reply = FReply::Handled().CaptureMouse(AsShared());
					}
					else
					{
						Reply = FReply::Handled();
					}
				}
			}
			else
			{
				if (bFingerOwningTouchInteraction.IsSet() /* && HasMouseCaptureByUser(MouseEvent.GetUserIndex(), MouseEvent.GetPointerIndex()) */)
				{
					CachedScroll -= ScrollByAmountLocal * Style->ScrollSensitivity;
					if (FMath::Abs(CachedScroll) >= 1.f)
					{
						SetSelectedIndex(SelectedIndex + FMath::TruncToInt(CachedScroll));
						CachedScroll = FMath::Fractional(CachedScroll);
					}

					Reply = FReply::Handled();
				}
			}

			return Reply;
		}
	}
	else
	{
		const float Pos = GetScrollComponentFromVector(MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition()));
		SetSelectedIndexByPosition(Pos);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void SInnoCardViewPanel::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsTouchEvent())
	{
		if (!bFingerOwningTouchInteraction.IsSet())
		{
			// If we currently do not have touch capture, allow this widget to begin scrolling on pointer enter events
			// if it comes from a child widget
			if (MyGeometry.IsUnderLocation(MouseEvent.GetLastScreenSpacePosition()))
			{
				bFingerOwningTouchInteraction = MouseEvent.GetPointerIndex();
			}
		}
	}
	else
	{
		SetIsSelected(true);
	}
}

void SInnoCardViewPanel::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, FString::Printf(TEXT("OnMouseLeave: %f, %f"), MouseEvent.GetCursorDelta().X, MouseEvent.GetCursorDelta().Y));
	
	if (MouseEvent.IsTouchEvent())
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, TEXT("\tIsTouchEvent"));

		if (!HasMouseCapture())
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, TEXT("\tHas no mouse capture"));
			bFingerOwningTouchInteraction.Reset();
		}

		if (bHasNotJustTouched)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, TEXT("\tbHasNotJustTouched"));
			bProbablyLostFocus = true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, TEXT("\tbHasJustTouched"));
			bHasNotJustTouched = true;
		}
	}
	else
	{
		SetIsSelected(false);
	}
}

FReply SInnoCardViewPanel::OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("OnTouchEnded"));

	if (bIsSelected && bTouchPanningCapture)
	{
		PendingScrollTriggerAmount = 0;
		CachedScroll = 0;
		bFingerOwningTouchInteraction.Reset();
		bTouchPanningCapture = false;

		Invalidate(EInvalidateWidget::Layout);

		return FReply::Handled().ReleaseMouseCapture();

	}
	else
	{
		const float Pos = GetScrollComponentFromVector(MyGeometry.AbsoluteToLocal(InTouchEvent.GetScreenSpacePosition()));
		SetSelectedIndexByPosition(Pos);

		SetIsSelected(true);

		return FReply::Handled().ReleaseMouseCapture();
	}

	return FReply::Unhandled();
}

void SInnoCardViewPanel::OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("OnMouseCaptureLost"));
}

void SInnoCardViewPanel::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::White, TEXT("OnFocusLost"));
}
