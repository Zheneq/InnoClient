// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "GMInno.generated.h"

/**
 * 
 */
UCLASS()
class INNOCLIENT_API AGMInno : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "InnoLow")
		class URequester* Requester;

	UPROPERTY(BlueprintReadOnly, Category = "InnoLow")
		class UInnoCards* Cards;


	UPROPERTY(EditAnywhere)
		FString PlayerName;
	UPROPERTY(EditAnywhere)
		FString PlayerPronoun;


	UFUNCTION(BlueprintCallable, Category = "InnoLow")
		FString BuildLoginData() const;

	UFUNCTION(BlueprintCallable, Category = "InnoLow")
		FString BuildProposeJson(const TArray<FString>& OtherPlayers, bool bEchoes, bool bFigures, bool bCities, bool bExtra) const;

	UFUNCTION(BlueprintCallable, Category = "InnoLow")
		FString BuildReplyJson(int32 RequestId, const TArray<FString>& Reply) const;


	// Sets default values for this actor's properties
	AGMInno();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable, Category = "InnoLow")
		bool InnoUpdate(FString JsonUpdate);
	
	// LOBBY
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLobbyDelegate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLobbyDelegateString, const FString&, String);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FLobbyDelegateUser, const FString&, Level, const FString&, Name, const FString&, AvatarURL);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FLobbyDelegatePlayer, const FString&, Name, const FString&, ID, bool, bUnavailable, const FString&, Status, bool, bRegistered, const FString&, Unk, const FString&, Level);

	UPROPERTY(BlueprintReadOnly, Category = "Lobby")
		int32 LobbyLoopNextRequestId;

	UPROPERTY(BlueprintReadOnly, Category = "Lobby")
		int32 LobbyNextChooseId;

	void InnoShowLobby(const TSharedPtr<FJsonObject> Object);
	void InnoHideLobby(const TSharedPtr<FJsonObject> Object);

	UPROPERTY(BlueprintAssignable, Category = "Lobby")
		FLobbyDelegate Lobby_Reset;

	UPROPERTY(BlueprintAssignable, Category = "Lobby")
		FLobbyDelegate HideLobby;

	// My status
	UPROPERTY(BlueprintAssignable, Category = "Lobby")
		FLobbyDelegateString Lobby_Status;

	// My info
	UPROPERTY(BlueprintAssignable, Category = "Lobby")
		FLobbyDelegateUser Lobby_User;

	// Lobby description
	UPROPERTY(BlueprintAssignable, Category = "Lobby")
		FLobbyDelegateString Lobby_Desc;

	// Other players data (one player per call)
	UPROPERTY(BlueprintAssignable, Category = "Lobby")
		FLobbyDelegatePlayer Lobby_Player;

	// CHAT & LOG
	void InnoSay(const TSharedPtr<FJsonObject> Object);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInnoDelegateChat, int32, Timestamp, const FString&, String);

	// Message in chat
	UPROPERTY(BlueprintAssignable, Category = "Inno")
		FInnoDelegateChat Chat;

	// Message in log
	UPROPERTY(BlueprintAssignable, Category = "Inno")
		FLobbyDelegateString Log;

	// PROMPTS
	void InnoChoose(const TSharedPtr<FJsonObject> Object);

	void InnoCancelChoose(const TSharedPtr<FJsonObject> Object);


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FInnoDelegateChoose, int32, RequestId, const FString&, Prompt, const TArray<FString>&, Choices, const TArray<FString>&, ChoiceKeys, int32, MinCount, int32, MaxCount);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInnoDelegateCancelChoose, int32, RequestId);

	// User prompt
	UPROPERTY(BlueprintAssignable, Category = "Inno")
		FInnoDelegateChoose Choose;

	// Cancel prompt (i.e. not answering for too long)
	UPROPERTY(BlueprintAssignable, Category = "Inno")
		FInnoDelegateCancelChoose CancelChoose;

	// GAME

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInnoDelegateInt, int32, Number);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FInnoDelegateGameSetup, int32, PlayerCount, int32, AchievementTarget, bool, bEchoes, bool, bFigures, bool, bCities);


	// Card list URL
	void InnoCardInfo(const TSharedPtr<FJsonObject> Object);

	UPROPERTY(BlueprintAssignable, Category = "Lobby")
		FLobbyDelegateString Inno_Cards;

	// Player count
	void InnoPlayerCount(const TSharedPtr<FJsonObject> Object);

	UPROPERTY(BlueprintAssignable, Category = "Lobby")
		FInnoDelegateInt Inno_PlayerCount;

	// Game setup
	void InnoGameSetup(const TSharedPtr<FJsonObject> Object);

	UPROPERTY(BlueprintAssignable, Category = "Lobby")
		FInnoDelegateGameSetup Inno_GameSetup;

};
