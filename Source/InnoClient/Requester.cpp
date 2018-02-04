// Fill out your copyright notice in the Description page of Project Settings.

#include "Requester.h"
#include "InnoClient.h"

URequester::URequester()
{
	//When the object is constructed, Get the HTTP module
	Http = &FHttpModule::Get();
}

void URequester::Request(const FHttpResponseDelegate& OnResponseCallback, FString Page, const FString& Verb, const FString& Content)
{
	if (CurrentRequest.IsValid())
	{
		FHttpRequestPtr Req = CurrentRequest.Pin();
		Req->CancelRequest();
		UE_LOG(LogTemp, Warning, TEXT("Request cancelled: %s + %d bytes"), *Req->GetURL(), Req->GetContentLength());
		OnResponse.Clear();
	}

	FHttpRequestPtr Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &URequester::Response);
	OnResponse = OnResponseCallback;
	//This is the url on which to process the request
	Request->SetURL(FString::Printf(TEXT("https://innovation.isotropic.org/%s"), *Page));
	Request->SetVerb(Verb);
	Request->SetContentAsString(Content);
	Request->SetHeader(L"Host", L"innovation.isotropic.org");
	//	Request->SetHeader(L"User-Agent", L"X-UnrealEngine-Agent");
	Request->SetHeader(L"User-Agent", L"Mozilla / 5.0 (Windows NT 6.1; WOW64; rv:53.0) Gecko / 20100101 Firefox / 53.0");
	//	Request->SetHeader(L"Content-Type", L"application/json");
	//Request->SetHeader(L"Accept-Encoding", L"gzip, deflate, br");
	//Request->SetHeader(L"Accept-Language", L"en-US,en;q=0.5");
	//Request->SetHeader(L"Accept", L"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
	Request->SetHeader(L"Connection", L"keep-alive");
	Request->SetHeader(L"Content-Type", L"application/x-www-form-urlencoded");
//	Request->SetHeader(L"Referer", LastURL);
//	LastURL = Request->GetURL();
	//	Request->SetHeader(L"Cookie", PlayerCookie);
	Request->ProcessRequest();
	CurrentRequest = Request;

	UE_LOG(LogTemp, Log, TEXT("Request: %s"), *Request->GetURL());
	UE_LOG(LogTemp, Log, TEXT("Content: %s"), *Content);
	auto headers = Request->GetAllHeaders();
	for (const auto& h : headers)
	{
		// GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Red, h);
		UE_LOG(LogTemp, Log, TEXT("Request header: %s"), *h);
	}


	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Emerald, TEXT("Request"));
}

void URequester::Response(FHttpRequestPtr Req, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (CurrentRequest.Pin() != Req)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unexpected response for: %s"), *Req->GetURL());
		return;
	}

	CurrentRequest.Reset();
	FHttpResponseDelegate Callback = OnResponse;
	OnResponse.Clear();

	if (bWasSuccessful)
	{

		UE_LOG(LogTemp, Log, TEXT("ResponseBP: %d"), Response->GetResponseCode());
		auto headers = Response->GetAllHeaders();
		for (const auto& h : headers)
		{
			// GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Red, h);
			UE_LOG(LogTemp, Log, TEXT("Response header: %s"), *h);
		}

		//TArray<ANSICHAR> ZeroTerminatedPayload(Response->GetContent());
		//ZeroTerminatedPayload.Add(0);
		//FString ContentAsString = ANSI_TO_TCHAR(ZeroTerminatedPayload.GetData());
		FString ContentAsString = Response->GetContentAsString();

		UE_LOG(LogTemp, Log, TEXT("Response content: %s"), *ContentAsString);
		Callback.ExecuteIfBound(Response->GetResponseCode(), ContentAsString, bWasSuccessful);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No response"));
		Callback.ExecuteIfBound(0, TEXT(""), bWasSuccessful);
	}
}

void URequester::LightRequest(FString Page, const FString& Content)
{
	FHttpRequestPtr Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &URequester::LightRequestResponse);
	Request->SetURL(FString::Printf(TEXT("https://innovation.isotropic.org/%s"), *Page));
	Request->SetVerb(TEXT("POST"));
	Request->SetContentAsString(Content);
	Request->SetHeader(L"Host", L"innovation.isotropic.org");
	//	Request->SetHeader(L"User-Agent", L"X-UnrealEngine-Agent");
	Request->SetHeader(L"User-Agent", L"Mozilla / 5.0 (Windows NT 6.1; WOW64; rv:53.0) Gecko / 20100101 Firefox / 53.0");
	Request->SetHeader(L"Connection", L"keep-alive");
	Request->SetHeader(L"Content-Type", L"application/json");
	Request->ProcessRequest();

	UE_LOG(LogTemp, Log, TEXT("Request: %s"), *Request->GetURL());
	UE_LOG(LogTemp, Log, TEXT("Content: %s"), *Content);
	auto headers = Request->GetAllHeaders();
	for (const auto& h : headers)
	{
		UE_LOG(LogTemp, Log, TEXT("Request header: %s"), *h);
	}
}

void URequester::LightRequestResponse(FHttpRequestPtr Req, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		FString ContentAsString = Response->GetContentAsString();

		UE_LOG(LogTemp, Log, TEXT("Light response content: %s"), *ContentAsString);

		OnLightRequestResponse.Broadcast(Response->GetResponseCode(), ContentAsString, bWasSuccessful);
	}
}

