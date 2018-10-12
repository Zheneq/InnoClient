// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Requester.generated.h"

/**
 * 
 */
UCLASS()
class INNOCLIENT_API URequester : public UObject
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_DELEGATE_ThreeParams(FHttpResponseDelegate, int32, Code, FString, Content, bool, bWasSuccessful);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHttpResponseDelegateMC, int32, Code, FString, Content, bool, bWasSuccessful);

private:

	FHttpModule* Http;

	TWeakPtr<IHttpRequest> CurrentRequest;
	FHttpResponseDelegate OnResponse;
	void Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void LightRequestResponse(FHttpRequestPtr Req, FHttpResponsePtr Response, bool bWasSuccessful);

	TWeakPtr<IHttpRequest> CurrentGetPageRequest;
	FHttpResponseDelegate OnGetPageResponse;
	void GetPageResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
public:
	URequester();

	UFUNCTION(BlueprintCallable, Category = "Requester")
		void Request(const FHttpResponseDelegate& OnResponseCallback, FString Page, const FString& Verb = "POST", const FString& Content = "");


	UFUNCTION(BlueprintCallable, Category = "Requester")
		void LightRequest(FString Page, const FString& Content);

	// Returns false if there is an active request
	UFUNCTION(BlueprintCallable, Category = "Requester")
		bool GetPage(const FHttpResponseDelegate& OnResponseCallback, FString Page);

	UPROPERTY(BlueprintAssignable, Category = "InnoLow")
		FHttpResponseDelegateMC OnLightRequestResponse;

	UPROPERTY(BlueprintReadOnly, Category = "InnoLow")
		FString CookieId;
};
