// Fill out your copyright notice in the Description page of Project Settings.

#include "GMInno.h"
#include "InnoClient.h"
#include "Requester.h"
#include "InnoCards.h"
#include "CardWidgetManager.h"
#include "InnoFunctionLibrary.h"

// Sets default values
AGMInno::AGMInno()
{
	Requester = nullptr;
	Cards = nullptr;
	CardWidgetManager = nullptr;
	LastUpdateId = -1;
}

// Called when the game starts or when spawned
void AGMInno::BeginPlay()
{
	Requester = NewObject<URequester>(this);
	Cards = NewObject<UInnoCards>(this);
	CardWidgetManager = NewObject<UCardWidgetManager>(this);
	CardWidgetManager->Init(Cards->GetCards().Num());

	Super::BeginPlay();
}


bool AGMInno::InnoTick(FString JsonUpdate)
{
	bReconnected = false;

	TSharedPtr<FJsonValue> JsonArray;

	if (UInnoFunctionLibrary::ParseJson(JsonUpdate, JsonArray) && JsonArray->AsArray().Num() >= 2)
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
			else if (Command == TEXT("player_count"))
			{
				InnoPlayerCount(Object);
			}
			else if (Command == TEXT("game_setup"))
			{
				InnoGameSetup(Object);
			}
			else if (Command == TEXT("supply"))
			{
				InnoSupply(Object);
			}
			else if (Command == TEXT("update"))
			{
				// TODO: Defer updates till the end of the loop? Watch out for plays, they contain updates too
				InnoUpdate(Object);
			}
			else if (Command == TEXT("play"))
			{
				InnoPlay(Object);
			}
			else if (Command == TEXT("ping"))
			{
				// do nothing, we respond to every signal
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoTick: Unknown command: %s."), *Command);
			}
			
		}
	}
	else
	{
		if (JsonUpdate == TEXT("You've reconnected from another browser window."))
		{
			bReconnected = true;
		}
		// TODO fix string. Don't forget to set to false in the beginning of the function if you add this.
		// if (JsonUpdate == TEXT("No user logon."))
		// {
		// 	bLoggedOut = true;
		// }

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
					FString(), //(*Player)[5]->AsString(), // ??
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
		// Url is supposed to change if card list is updated
		if (Url != CardsUrl)
		{
			CardsUrl = Url;
			Inno_Cards.Broadcast(Url);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("AGMInno::InnoCardInfo: Up to date. (%s)"), *CardsUrl);
		}
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
		PlayerNum = PlayerCount;
		Inno_PlayerCount.Broadcast(PlayerCount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoPlayerCount: Unsupported parameters."));
	}
}

void AGMInno::InnoGameSetup(const TSharedPtr<FJsonObject> Object)
{
	int32 PlayerCount = 0, AchievementTarget = 0;
	bool bEchoes = false, bFigures = false, bCities = false;

	check(Object.IsValid());

	if (Object->TryGetNumberField(TEXT("player_count"), PlayerCount) &&
		Object->TryGetNumberField(TEXT("achievement_target"), AchievementTarget) &&
		Object->TryGetBoolField(TEXT("show_echoes"), bEchoes) &&
		Object->TryGetBoolField(TEXT("show_figures"), bFigures) &&
		Object->TryGetBoolField(TEXT("show_cities"), bCities))
	{
		PlayerNum = PlayerCount;
		Inno_GameSetup.Broadcast(PlayerCount, AchievementTarget, bEchoes, bFigures, bCities);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoGameSetup: Unsupported parameters."));
	}
}

void AGMInno::InnoSupply(const TSharedPtr<FJsonObject> Object)
{
	check(Object.IsValid());

	const TSharedPtr<FJsonObject>* State = nullptr;
	if (Object->TryGetObjectField(TEXT("state"), State))
	{
		const TArray< TSharedPtr<FJsonValue> > *Achievements = nullptr, *Piles = nullptr;
		if (State->Get()->TryGetArrayField(TEXT("ach"), Achievements) &&
			State->Get()->TryGetArrayField(TEXT("piles"), Piles))
		{
			TArray<int32> _ach, _pil;

			for (const auto& a : *Achievements)
			{
				_ach.Add(a.Get()->AsNumber());
			}
			for (const auto& p : *Piles)
			{
				_pil.Add(p.Get()->AsNumber());
			}

			Inno_Supply.Broadcast(_ach, _pil);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoSupply: Unsupported parameters."));
		}
	}
}

void AGMInno::InnoUpdate(const TSharedPtr<FJsonObject> Object)
{
	check(Object.IsValid());

	int32 UpdateId = Object->GetIntegerField(TEXT("id"));
	if (UpdateId < LastUpdateId)
	{
		// TODO Sometimes discards needed data
		UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoUpdate: Outdated. Discarding."));
		return;
	}
	LastUpdateId = UpdateId;

	const TSharedPtr<FJsonObject>* Update = nullptr;
	if (Object->TryGetObjectField(TEXT("update"), Update))
	{
		InnoUpdatePlayer(*Update, -1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoUpdate: Unsupported parameters."));
	}
}

void AGMInno::InnoUpdatePlayer(const TSharedPtr<FJsonObject> Object, int32 Id)
{
	check(Object.IsValid());

	FInnoPlayerInfo PlayerInfo;
	if (FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(), &PlayerInfo))
	{
		if (Id == -1) // it is me
		{
			const TSharedPtr<FJsonObject>* OpponentUpdate = nullptr;
			if (Object->TryGetObjectField(TEXT("opp"), OpponentUpdate))
			{
				const TSharedPtr<FJsonObject>* Opponent = nullptr;
				for (int32 i = 0; i < PlayerNum - 1; ++i)
				{
					const FString OpponentId = FString::FromInt(i);
					if (OpponentUpdate->Get()->TryGetObjectField(OpponentId, Opponent))
					{
						InnoUpdatePlayer(*Opponent, i);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoUpdatePlayer: Incorrect opponent."));
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoUpdatePlayer: No opponents."));
			}
		}

		if (FInnoPlayerInfo::JsonHasAnyField(Object))
		{
			Inno_UpdatePlayer.Broadcast(Id, PlayerInfo);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoUpdatePlayer: Unsupported parameters."));
	}
}

void AGMInno::InnoPlay(const TSharedPtr<FJsonObject> Object)
{
	check(Object.IsValid());

	InnoUpdate(Object);

	int32 PlayId = 0, DrawDeck = 0;
	TArray<int32> Achs, Decree;
	TArray<EInnoColor> Endorse, Inspire;

	const TArray<TSharedPtr<FJsonValue>>* ArrayParam;

	if (Object->TryGetArrayField(TEXT("achs"), ArrayParam))
	{
		for (const auto& ach : *ArrayParam)
		{
			Achs.Add(ach->AsNumber());
		}
	}

	if (Object->TryGetArrayField(TEXT("decrees"), ArrayParam))
	{
		for (const auto& ach : *ArrayParam)
		{
			Decree.Add(ach->AsNumber());
		}
	}

	if (Object->TryGetArrayField(TEXT("inspire"), ArrayParam))
	{
		for (const auto& ach : *ArrayParam)
		{
			Inspire.Add(UInnoFunctionLibrary::ColorFromString(this, ach->AsString()));
		}
	}

	if (Object->TryGetArrayField(TEXT("endorse"), ArrayParam))
	{
		for (const auto& ach : *ArrayParam)
		{
			Endorse.Add(UInnoFunctionLibrary::ColorFromString(this, ach->AsString()));
		}
	}

	if (Object->TryGetNumberField(TEXT("draw"), DrawDeck) &&
		Object->TryGetNumberField(TEXT("id"), PlayId))
	{
		Inno_Play.Broadcast(PlayId, DrawDeck, Achs, Decree, Inspire, Endorse);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGMInno::InnoPlay: Unsupported parameters."));
	}
}


FString AGMInno::BuildLoginData(FString PlayerName, EInnoPlayerPronoun PlayerPronoun)
{
	return FString::Printf(TEXT("name=%s&pronoun=%c&lobby=casual&status=&drag=on"), *PlayerName, static_cast<char>(PlayerPronoun));
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


FString AGMInno::BuildPlayJson(int32 RequestId, EInnoPlayAction Action, int32 IntParam, EInnoColor ColorParam, bool bConfirm) const
{
	FString StrAction, StrPayload;

	switch (ColorParam)
	{
	case EInnoColor::IC_Red:
		StrPayload = TEXT("\"red\"");
		break;
	case EInnoColor::IC_Green:
		StrPayload = TEXT("\"green\"");
		break;
	case EInnoColor::IC_Blue:
		StrPayload = TEXT("\"blue\"");
		break;
	case EInnoColor::IC_Yellow:
		StrPayload = TEXT("\"yellow\"");
		break;
	case EInnoColor::IC_Purple:
		StrPayload = TEXT("\"purple\"");
		break;
	}

	switch (Action)
	{
	case EInnoPlayAction::IPA_Draw:
		StrAction = TEXT("draw");
		StrPayload = TEXT("null");
		break;
	case EInnoPlayAction::IPA_Meld:
		StrAction = TEXT("meld");
		StrPayload = FString::Printf(TEXT("%d"), IntParam);
		break;
	case EInnoPlayAction::IPA_Dogma:
		StrAction = TEXT("dogma");
		break;
	case EInnoPlayAction::IPA_Achieve:
		StrAction = TEXT("achieve");
		StrPayload = FString::Printf(TEXT("%d"), IntParam);
		break;
	case EInnoPlayAction::IPA_Inspire:
		StrAction = TEXT("inspire");
		break;
	case EInnoPlayAction::IPA_Decree:
		StrAction = TEXT("decree");
		StrPayload = FString::Printf(TEXT("%d"), IntParam);
		break;
	case EInnoPlayAction::IPA_Endorse:
		StrAction = TEXT("endorse");
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("AGMInno::BuildPlayJson: Unsupported parameters."));
	}

	return FString::Printf(TEXT("{\"id\":%d,\"what\":\"%s\",\"extra\":%s,\"confirm\":false}"), RequestId, *StrAction, *StrPayload);

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
		else if (String == TEXT("replace"))
		{
			Log.Broadcast(Object->GetStringField(TEXT("message")), true);
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
		Log.Broadcast(Object->GetStringField(TEXT("message")), false);
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
