// Fill out your copyright notice in the Description page of Project Settings.

#include "InnoCardView.h"
#include "Widgets/Styles/InnoCardViewWidgetStyle.h"
#include "InnoCardViewSlot.h"

UInnoCardView::UInnoCardView(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SInnoCardViewPanel::FArguments Defaults;

	bIsVariable = true;
	Orientation = Defaults._Orientation;
	bAnimateScroll = Defaults._bAnimateScroll;
	StyleOverride = nullptr;

	DefaultSelection = 0;
	bDefaultSelected = true;

//	UInnoCardView::FArguments Defaults;
//	Visibility = UWidget::ConvertRuntimeToSerializedVisibility(Defaults._Visibility.Get());
}

void UInnoCardView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyInnoCardViewPanel.IsValid())
	{
		MyInnoCardViewPanel->SetOrientation(Orientation);
		MyInnoCardViewPanel->bAnimateScroll = bAnimateScroll;
		MyInnoCardViewPanel->SetStyle(IsValid(StyleOverride) ? StyleOverride->GetStyleChecked<FInnoCardViewStyle>(): nullptr);
	}

}

#if WITH_EDITOR

void UInnoCardView::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (MyInnoCardViewPanel.IsValid())
	{
		MyInnoCardViewPanel->SetSelectedIndex(DefaultSelection);
		MyInnoCardViewPanel->SetIsSelected(bDefaultSelected);
	}
}

#endif // WITH_EDITOR

void UInnoCardView::Select(int32 Index)
{
	if (MyInnoCardViewPanel.IsValid())
	{
		MyInnoCardViewPanel->SetSelectedIndex(Index);
	}
}

void UInnoCardView::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyInnoCardViewPanel.Reset();
}

UClass* UInnoCardView::GetSlotClass() const
{
	return UInnoCardViewSlot::StaticClass();
}

void UInnoCardView::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if (MyInnoCardViewPanel.IsValid())
	{
		CastChecked<UInnoCardViewSlot>(InSlot)->BuildSlot(MyInnoCardViewPanel.ToSharedRef());
	}
}

void UInnoCardView::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if (MyInnoCardViewPanel.IsValid())
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if (Widget.IsValid())
		{
			MyInnoCardViewPanel->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

UInnoCardViewSlot* UInnoCardView::AddChildToCardView(UWidget* Content)
{
	return Cast<UInnoCardViewSlot>(Super::AddChild(Content));
}

TSharedRef<SWidget> UInnoCardView::RebuildWidget()
{
	MyInnoCardViewPanel = SNew(SInnoCardViewPanel);

	for (UPanelSlot* PanelSlot : Slots)
	{
		if (UInnoCardViewSlot* TypedSlot = Cast<UInnoCardViewSlot>(PanelSlot))
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyInnoCardViewPanel.ToSharedRef());
		}
	}

	return MyInnoCardViewPanel.ToSharedRef();
}

#if WITH_EDITOR

const FText UInnoCardView::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "Panel", "Panel");
}

#endif
