#include "GameState/BombGameState.h"
#include "Global.h"
#include "Characters/BombCharacter.h"
#include "Controllers/BombController.h"
#include "GameModes/BombGameMode.h"
#include "Net/UnrealNetwork.h"

ABombGameState::ABombGameState()
{
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