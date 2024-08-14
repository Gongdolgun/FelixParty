#include "GameState/BombGameState.h"
#include "Global.h"
#include "Characters/BombCharacter.h"
#include "Controllers/BombController.h"
#include "Net/UnrealNetwork.h"

ABombGameState::ABombGameState()
{
	bReplicates = true;

}

void ABombGameState::BeginPlay()
{
	Super::BeginPlay();

}

void ABombGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ABombGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

int32 ABombGameState::GetPlayerScore(const FString& PlayerName) const
{
    for (const FPlayerInGameData& PlayerData : PlayerDatas)
    {
        if (PlayerData.PlayerName == PlayerName)
        {
            return PlayerData.Score; // 점수 반환
        }
    }

    return 0;
}


