#include "GameState/OnlyUpGameState.h"
#include "Global.h"
#include "Net/UnrealNetwork.h"

AOnlyUpGameState::AOnlyUpGameState()
{
	bReplicates = true;

}

void AOnlyUpGameState::BeginPlay()
{
	Super::BeginPlay();

}

void AOnlyUpGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AOnlyUpGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerRanking);
}

void AOnlyUpGameState::PlayerRank(APlayerController* InController)
{
	PlayerRanking.AddUnique(InController);

	
}





