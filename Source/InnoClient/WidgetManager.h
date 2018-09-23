// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UserWidget.h"
#include "WidgetManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class INNOCLIENT_API UWidgetManager : public UActorComponent
{
	GENERATED_BODY()

private:
	TArray<TWeakObjectPtr<UUserWidget>> Widgets;

public:
	/*  */
	UFUNCTION(BlueprintCallable)
		void Init(int32 Size);

	/* Make widget for given Id. This function is supposed to be overridden in BP. */
	UFUNCTION(BlueprintNativeEvent)
		UUserWidget* MakeWidget(int32 Id);

	/* Get widget for given Id. If it does not exist, it will be created. */
	UFUNCTION(BlueprintCallable)
		UUserWidget* GetWidget(int32 Id);
};
