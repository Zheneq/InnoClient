// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "Requester.h"
#include "JsonObjectConverter.h"
#include "GMInno.generated.h"

UENUM(BlueprintType)
enum EInno
{
	LOCAL_PLAYER_ID = -1		UMETA(DisplayName = "Local Player ID")
};

UENUM(BlueprintType)
enum class EInnoSplay : uint8
{
	IPA_None	= 0		UMETA(DisplayName = "No splay"),
	IPA_Left	= 1		UMETA(DisplayName = "Splay left"),
	IPA_Right	= 2		UMETA(DisplayName = "Splay right"),
	IPA_Up		= 3		UMETA(DisplayName = "Splay up"),
};

USTRUCT(BlueprintType)
struct FInnoPlayerPileInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Color"))
		FString Color;
	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Cards"))
		TArray<int32> Num;
	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Splay-revealed resources", ToolTip = "Range. [0] is first resource visible, [1] is first resource hidden."))
		TArray<int32> Splay;
	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Warning"))
		bool q;
};

USTRUCT(BlueprintType)
struct FInnoPlayerInfoIcons
{
	GENERATED_BODY()
		
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 Castle;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 Leaf;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 Lightbulb;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 Crown;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 Factory;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 Clock;
	UPROPERTY(BlueprintReadOnly, Category = "Inno")
		int32 Hex;

	FInnoPlayerInfoIcons() :
		Castle(-1), Leaf(-1), Lightbulb(-1), Crown(-1), Factory(-1), Clock(-1), Hex(-1)
	{}
};

USTRUCT(BlueprintType)
struct FInnoPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Player name"))
		FString Name;
	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Board"))
		TArray<FInnoPlayerPileInfo> Board;
	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Icons"))
		FInnoPlayerInfoIcons Icons;
	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Hand"))
		TArray<int32> Hand;
	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Forecast"))
		TArray<int32> Forecast;
	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Achievements"))
		TArray<int32> Ach;
	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Score pile"))
		TArray<int32> Score;
	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Bonuses"))
		TArray<int32> Bonus;
	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Total Score"))
		int32 Score_total;
	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Order"))
		int32 Order;
	UPROPERTY(BlueprintReadOnly, Category = "Inno", meta = (DisplayName = "Extra achievements"))
		TArray<int32> Extra_ach;

	static bool JsonHasAnyField(const TSharedPtr<FJsonObject> Object)
	{
		return Object->HasField(TEXT("Name")) ||
			Object->HasField(TEXT("Board")) ||
			Object->HasField(TEXT("Icons")) ||
			Object->HasField(TEXT("Hand")) ||
			Object->HasField(TEXT("Forecast")) ||
			Object->HasField(TEXT("Ach")) ||
			Object->HasField(TEXT("Score")) ||
			Object->HasField(TEXT("Bonus")) ||
			Object->HasField(TEXT("Score_total")) ||
			Object->HasField(TEXT("Order")) ||
			Object->HasField(TEXT("Extra_ach"));
	}
};

UENUM(BlueprintType)
enum class EInnoPlayAction : uint8
{
	IPA_Draw		UMETA(DisplayName = "Draw"),
	IPA_Meld		UMETA(DisplayName = "Meld"),
	IPA_Dogma		UMETA(DisplayName = "Dogma"),
	IPA_Achieve		UMETA(DisplayName = "Achieve"),
	IPA_Inspire		UMETA(DisplayName = "Inspire"),
	IPA_Decree		UMETA(DisplayName = "Decree"),
	IPA_Endorse		UMETA(DisplayName = "Endorse"),
};

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

	UPROPERTY(BlueprintReadOnly, Category = "InnoLow")
		class UCardWidgetManager* CardWidgetManager;

	// Where we got the cards from
	UPROPERTY(BlueprintReadOnly, Category = "InnoLow")
		FString CardsUrl;

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

	UFUNCTION(BlueprintCallable, Category = "InnoLow")
		FString BuildPlayJson(int32 RequestId, EInnoPlayAction Action, int32 IntParam, EInnoColor ColorParam, bool bConfirm) const;


	// Sets default values for this actor's properties
	AGMInno();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable, Category = "InnoLow")
		bool InnoTick(FString JsonUpdate);
	
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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInnoDelegateLog, const FString&, String, bool, bReplaceLastMessage);

	// Message in log
	UPROPERTY(BlueprintAssignable, Category = "Inno")
		FInnoDelegateLog Log;

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

	// Number of players in game
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inno")
		int32 PlayerNum;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInnoDelegateInt, int32, Number);


	// Card list URL
	void InnoCardInfo(const TSharedPtr<FJsonObject> Object);

	// Called if Cards URL has changed
	UPROPERTY(BlueprintAssignable, Category = "Game")
		FLobbyDelegateString Inno_Cards;

	// Player count
	void InnoPlayerCount(const TSharedPtr<FJsonObject> Object);

	UPROPERTY(BlueprintAssignable, Category = "Game")
		FInnoDelegateInt Inno_PlayerCount;

	// Game setup
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FInnoDelegateGameSetup, int32, PlayerCount, int32, AchievementTarget, bool, bEchoes, bool, bFigures, bool, bCities);

	void InnoGameSetup(const TSharedPtr<FJsonObject> Object);

	UPROPERTY(BlueprintAssignable, Category = "Game")
		FInnoDelegateGameSetup Inno_GameSetup;

	// Supply
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInnoDelegateSupply, const TArray<int32>&, AvailableAchievements, const TArray<int32>&, Piles);

	void InnoSupply(const TSharedPtr<FJsonObject> Object);

	UPROPERTY(BlueprintAssignable, Category = "Game")
		FInnoDelegateSupply Inno_Supply;

	// Update
	int32 LastUpdateId;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInnoDelegateUpdatePlayer, int32, Id, const FInnoPlayerInfo&, PlayerInfo);

	// Update everything on the table
	void InnoUpdate(const TSharedPtr<FJsonObject> Object);

	// Update specific player
	void InnoUpdatePlayer(const TSharedPtr<FJsonObject> Object, int32 Id = -1);

	UPROPERTY(BlueprintAssignable, Category = "Game")
		FInnoDelegateUpdatePlayer Inno_UpdatePlayer;

	// Update + Player expected to make their move
	void InnoPlay(const TSharedPtr<FJsonObject> Object);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FInnoDelegatePlay, int32, PlayId, int32, DrawDeck, const TArray<int32>&, Achs, const TArray<int32>&, Decree, const TArray<EInnoColor>&, Inspire, const TArray<EInnoColor>&, Endorse);
	UPROPERTY(BlueprintAssignable, Category = "Game")
		FInnoDelegatePlay Inno_Play;
};
