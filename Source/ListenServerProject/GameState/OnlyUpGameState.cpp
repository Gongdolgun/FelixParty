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

int32 AOnlyUpGameState::PlayerRank(APlayerController* InController)
{
	if (InController == nullptr) return 0.0f; 

	if (!PlayerRanking.Contains(InController))
	{
		PlayerRanking.Add(InController);

		int32 index = PlayerRanking.Num() - 1; 

		if (index < RankScores.Num())
		{
			return RankScores[index]; 
		}
	}

	return 0;
}







