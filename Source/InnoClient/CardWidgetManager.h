// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UserWidget.h"
#include "CardWidgetManager.generated.h"

class SInnoCard;
/**
 * Creates card widgets on demand and keeps them for reusing.
 */
UCLASS(Blueprintable)
class INNOCLIENT_API UCardWidgetManager : public UObject
{
	GENERATED_BODY()

private:

	/* Widget array to use  */
	TArray<TSharedPtr<SInnoCard>> Widgets;

public:
	/*  */
	void Init(int32 Size);

	/* Get widget for given Id. If it does not exist, it will be created. */
	TSharedPtr<SInnoCard> GetCard(int32 Id);

};
