// Fill out your copyright notice in the Description page of Project Settings.

#include "GMInno.h"
#include "InnoClient.h"
#include "Requester.h"
#include "InnoCards.h"
#include "InnoFunctionLibrary.h"

// Sets default values
AGMInno::AGMInno()
{
	Requester = nullptr;
	Cards = nullptr;
	PlayerName = TEXT("4356274");
	PlayerPronoun = TEXT("M");
	// PlayerCookie = TEXT("");
}

// Called when the game starts or when spawned
void AGMInno::BeginPlay()
{
	Requester = NewObject<URequester>(this);
	Cards = NewObject<UInnoCards>(this);
	
	Super::BeginPlay();
}


bool AGMInno::InnoUpdate(FString JsonUpdate)
{
	TSharedPtr<FJsonValue> JsonArray;
	if (UInnoFunctionLibrary::ParseJson(JsonUpdate, JsonArray))
	{
		LobbyLoopNextRequestId = JsonArray->AsArray()[0]->AsNumber();
		for (const auto& _Object : JsonArray->AsArray()[1]->AsArray())
		{
			const TSharedPtr<FJsonObject> Object = _Object->AsObject();

			FString Command;
			Object->TryGetStringField(TEXT("command"), Command);

			if (Command == TEXT("show_lobby"))
			{
				InnoShowLobby(Object);
			}
			else if (Command == TEXT("say"))
			{
				InnoSay(Object);
			}
			else if (Command == TEXT("choose"))
			{
				InnoChoose(Object);
			}
			else if (Command == TEXT("cancel_choose"))
			{
				InnoCancelChoose(Object);
			}
			else if (Command == TEXT("hide_lobby"))
			{
				InnoHideLobby(Object);
			}
			else if (Command == TEXT("card_info"))
			{
				InnoCardInfo(Object);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoUpdate: Unknown command: %s."), *Command);
			}

		}
	}
	else
	{
		// return false only if Json is unparsed and not empty
		return JsonUpdate == TEXT("");
	}
	return true;
}

void AGMInno::InnoShowLobby(const TSharedPtr<FJsonObject> Object)
{
	FString StringParam, StringParam2;
	double DoubleParam;
	const TSharedPtr<FJsonObject>* JsonParam;
	const TArray<TSharedPtr<FJsonValue>>* ArrayParam;

	check(Object.IsValid());

	Lobby_Reset.Broadcast();

	if (Object->TryGetStringField(TEXT("status"), StringParam))
	{
		Lobby_Status.Broadcast(StringParam);
	}

	if (Object->TryGetObjectField(TEXT("user"), JsonParam) &&
		// JsonParam->Get()->TryGetNumberField(TEXT("level"), DoubleParam ) &&
		JsonParam->Get()->TryGetStringField(TEXT("name"), StringParam) &&
		JsonParam->Get()->TryGetStringField(TEXT("avatar"), StringParam2))
	{
		Lobby_User.Broadcast(TEXT(""), StringParam, StringParam2);
	}

	if (Object->TryGetStringField(TEXT("url"), StringParam))
	{
		InnoCardInfo(Object);
	}

	if (Object->TryGetStringField(TEXT("lobby_desc"), StringParam))
	{
		Lobby_Desc.Broadcast(StringParam);
	}

	if (Object->TryGetArrayField(TEXT("players"), ArrayParam))
	{
		for (const auto& _Player : *ArrayParam)
		{
			const TArray<TSharedPtr<FJsonValue>>* Player;
			if (_Player->TryGetArray(Player))
			{
				Lobby_Player.Broadcast(
					(*Player)[0]->AsString(),
					(*Player)[1]->AsString(),
					(*Player)[2]->AsBool(),
					(*Player)[3]->AsString(),
					(*Player)[4]->AsBool(),
					(*Player)[5]->AsString(), // ??
					(*Player)[6]->AsString()
				);
			}
		}
	}

	if (Object->TryGetNumberField(TEXT("id"), DoubleParam))
	{
		LobbyNextChooseId = DoubleParam;
	}

}

void AGMInno::InnoCardInfo(const TSharedPtr<FJsonObject> Object)
{
	FString Url;
	check(Object.IsValid());

	if (Object->TryGetStringField(TEXT("url"), Url))
	{
		Inno_Cards.Broadcast(Url);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoCardInfo: Unsupported parameters."));
	}
}

void AGMInno::InnoPlayerCount(const TSharedPtr<FJsonObject> Object)
{
	int32 PlayerCount;
	check(Object.IsValid());

	if (Object->TryGetNumberField(TEXT("player_count"), PlayerCount))
	{
		Inno_PlayerCount.Broadcast(PlayerCount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoPlayerCount: Unsupported parameters."));
	}
}

void AGMInno::InnoGameSetup(const TSharedPtr<FJsonObject> Object)
{
	int32 PlayerCount, AchievementTarget;
	bool bEchoes, bFigures, bCities;

	check(Object.IsValid());

	if (Object->TryGetNumberField(TEXT("player_count"),       PlayerCount) &&
		Object->TryGetNumberField(TEXT("achievement_target"), AchievementTarget) &&
		Object->TryGetBoolField(TEXT("show_echoes"),          bEchoes) &&
		Object->TryGetBoolField(TEXT("show_figures"),         bFigures) &&
		Object->TryGetBoolField(TEXT("show_cities"),          bCities))
	{
		Inno_GameSetup.Broadcast(PlayerCount, AchievementTarget, bEchoes, bFigures, bCities);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoGameSetup: Unsupported parameters."));
	}
}


FString AGMInno::BuildLoginData() const
{
	return FString::Printf(TEXT("name=%s&pronoun=%s&lobby=casual&status=&drag=on"), *PlayerName, *PlayerPronoun);
}


FString AGMInno::BuildProposeJson(const TArray<FString>& OtherPlayers, bool bEchoes, bool bFigures, bool bCities, bool bExtra) const
{
	FString Others;
	bool bFirst = true;
	for (const FString& Other : OtherPlayers)
	{
		Others = FString::Printf(TEXT("%s%s \"%s\""), *Others, bFirst ? TEXT("") : TEXT(","), *Other);
		bFirst = false;
	}

	return FString::Printf(TEXT("{\"id\":%d,\"others\":[%s],\"echoes\":%s,\"figures\":%s,\"cities\":%s,\"extra\":%s}"),
		LobbyNextChooseId,
		*Others,
		bEchoes ? TEXT("true") : TEXT("false"),
		bFigures ? TEXT("true") : TEXT("false"),
		bCities ? TEXT("true") : TEXT("false"),
		bExtra ? TEXT("true") : TEXT("false"));
}

FString AGMInno::BuildReplyJson(int32 RequestId, const TArray<FString>& Reply) const
{
	FString Answer;
	bool bFirst = true;
	for (const FString& R : Reply)
	{
		Answer = FString::Printf(TEXT("%s%s \"%s\""), *Answer, bFirst ? TEXT("") : TEXT(","), *R);
		bFirst = false;
	}

	return FString::Printf(TEXT("{\"id\":%d,\"answer\":[%s]}"), RequestId, *Answer);
}


void AGMInno::InnoSay(const TSharedPtr<FJsonObject> Object)
{
	check(Object.IsValid());

	//TODO: marker

	FString String;

	if (Object->TryGetStringField(TEXT("extra"), String))
	{
		if (String == TEXT("chat"))
		{
			Chat.Broadcast(Object->GetIntegerField(TEXT("timestamp")), Object->GetStringField(TEXT("message")));
		}
		else if (String == TEXT("temp"))
		{
			UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoSay [Temp]: \"%s\"."), *Object->GetStringField(TEXT("message")));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoSay: Unsupported parameters."));
		}
	}
	else
	{
		Log.Broadcast(Object->GetStringField(TEXT("message")));
	}
}

void AGMInno::InnoChoose(const TSharedPtr<FJsonObject> Object)
{
	check(Object.IsValid());

	//TODO: from_supply

	FString Prompt;
	TArray<FString> Choices;
	// suspend_propose

	if (Object->TryGetStringField(TEXT("prompt"), Prompt) && Object->TryGetStringArrayField(TEXT("choices"), Choices))
	{
		int32 ChooseId = Object->GetIntegerField(TEXT("id"));

		TArray<FString> ChoiceKeys;
		bool bHasKeys = Object->TryGetStringArrayField(TEXT("choice_keys"), ChoiceKeys);

		Choose.Broadcast(ChooseId, Prompt, Choices, bHasKeys ? ChoiceKeys : Choices, Object->GetIntegerField(TEXT("min")), Object->GetIntegerField(TEXT("max")));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoChoose: Unsupported parameters."));
	}
}

void AGMInno::InnoCancelChoose(const TSharedPtr<FJsonObject> Object)
{
	check(Object.IsValid());

	int32 ChooseId = Object->GetIntegerField(TEXT("id"));
	CancelChoose.Broadcast(ChooseId);
}

void AGMInno::InnoHideLobby(const TSharedPtr<FJsonObject> Object)
{
	// check(Object.IsValid());

	// FString CardsURL = Object->GetStringField(TEXT("url"));
	HideLobby.Broadcast();
}
