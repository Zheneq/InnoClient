// Fill out your copyright notice in the Description page of Project Settings.

#include "CardWidgetManager.h"
#include "InnoClient.h"
#include "InnoFunctionLibrary.h"
#include "Widgets/Slate/SInnoCard.h"

void UCardWidgetManager::Init(int32 Size)
{
//	Widgets.Empty();
//	Widgets.SetNum(Size);
}

TSharedPtr<SInnoCard> UCardWidgetManager::GetCard(int32 Id)
{
//	if (!Widgets.IsValidIndex(Id))
//	{
//		UE_LOG(LogInno, Warning, TEXT("UWidgetManager::GetCard: Id = %d is out of range!"), Id);
//		return TSharedPtr<SInnoCard>();
//	}
//
//	if (Widgets[Id].IsValid())
//	{
//		return Widgets[Id];
//	}
//	else
//	{
//		UE_LOG(LogInno, Log, TEXT("UWidgetManager::GetCard: Creating widget %d!"), Id);
		
		TSharedPtr<SInnoCard> Card = SNew(SInnoCard).Card(UInnoFunctionLibrary::GetCard(this, Id));

//		if (Card)
//		{
//			Widgets[Id] = Card;
			return Card;
//		}
//	}
//
//	return TSharedPtr<SInnoCard>();
}
