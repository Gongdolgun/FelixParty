#include "DefaultGameState.h"
#include "Global.h"
#include "Net/UnrealNetwork.h"

ADefaultGameState::ADefaultGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	GameStateType = EGameStateType::GameStart;
}

void ADefaultGameState::BeginPlay()
{
	Super::BeginPlay();


}

void ADefaultGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetTimer(DeltaSeconds);

}

void ADefaultGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, GameStartTime);
	DOREPLIFETIME(ThisClass, GameMatchTime);
	DOREPLIFETIME(ThisClass, GameOverTime);

	DOREPLIFETIME(ThisClass, GameStateType);

	DOREPLIFETIME(ThisClass, PlayerDatas);
}

void ADefaultGameState::SetTimer(float InTime)
{
	switch (GameStateType)
	{
	case EGameStateType::GameStart:
		if (GameStartTime >= 0.0f)
		{
			GameStartTime -= InTime;
			if (GameStartTime <= 0.0f)
			{
				SetGameState(EGameStateType::GamePlay);
			}
		}

		break;

	case EGameStateType::GamePlay:
		if (GameMatchTime >= 0.0f)
		{
			GameMatchTime -= InTime;
			if (GameMatchTime <= 0.0f)
			{
				SetGameState(EGameStateType::GameOver);
			}
		}

		break;

	case EGameStateType::GameOver:
		if (GameOverTime >= 0.0f)
		{
			GameOverTime -= InTime;
		}

		break;
	}
}

void ADefaultGameState::SetGameState(EGameStateType InGameStateType)
{
	if (GameStateType != InGameStateType)
	{
		switch (InGameStateType)
		{
		case EGameStateType::GameStart:
			ChangeGameType(EGameStateType::GameStart);
			GameStartTime = 5.0f;

			break;
		case EGameStateType::GamePlay:
			ChangeGameType(EGameStateType::GamePlay);
			GameMatchTime = 10.0f;

			break;
		case EGameStateType::GameOver:
			ChangeGameType(EGameStateType::GameOver);
			GameOverTime = 5.0f;
			break;
		}
		
	}
}

EGameStateType ADefaultGameState::GetGameStateType()
{
	return GameStateType;
}

void ADefaultGameState::ChangeGameType(EGameStateType InGameType)
{
	EGameStateType prevGameType = GameStateType;
	GameStateType = InGameType;

	if (OnGameStateTypeChanged.IsBound())
		OnGameStateTypeChanged.Broadcast(prevGameType, GameStateType);
}

void ADefaultGameState::UpdatePlayerScore(const FString& PlayerName, int32 Score)
{
	for (FPlayerInGameData& PlayerData : PlayerDatas)
		if (PlayerData.PlayerName == PlayerName)
			PlayerData.Score += Score;
}

void ADefaultGameState::AddPlayerData(const FString& PlayerName, int32 Score, FColor PlayerColor)
{
	// 새 플레이어 추가
	PlayerDatas.Add(FPlayerInGameData(PlayerName, Score, PlayerColor));
}

FPlayerInGameData ADefaultGameState::GetPlayerData(FString PlayerName)
{
	for (FPlayerInGameData& PlayerData : PlayerDatas)
		if (PlayerData.PlayerName == PlayerName)
			return PlayerData;

	return FPlayerInGameData();
}