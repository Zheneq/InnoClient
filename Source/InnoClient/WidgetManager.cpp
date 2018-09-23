// Fill out your copyright notice in the Description page of Project Settings.

#include "WidgetManager.h"

void UWidgetManager::Init(int32 Size)
{
	Widgets.Empty();
	Widgets.SetNum(Size);
}

UUserWidget* UWidgetManager::MakeWidget_Implementation(int32 Id)
{
	// Do nothing by default -- we don't know what widget we are to make and how.
	return nullptr;
}

UUserWidget* UWidgetManager::GetWidget(int32 Id)
{
	if (!Widgets.IsValidIndex(Id))
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidgetManager::GetWidget: Id is out of range!"));
		return nullptr;
	}

	if (Widgets[Id].IsValid())
	{
		return Widgets[Id].Get();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidgetManager::GetWidget: Creating widget %d!"), Id);
		UUserWidget* NewWidget = MakeWidget(Id);
		if (NewWidget)
		{
			Widgets[Id] = NewWidget;
			return NewWidget;
		}
	}

	return nullptr;
}
