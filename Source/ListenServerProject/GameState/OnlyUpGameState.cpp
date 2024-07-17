#include "GameState/OnlyUpGameState.h"
#include "Global.h"

AOnlyUpGameState::AOnlyUpGameState()
{

}

void AOnlyUpGameState::BeginPlay()
{
	Super::BeginPlay();

}

void AOnlyUpGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AOnlyUpGameState::PlayerRank(APlayerController* InPlayerController)
{
	if (InPlayerController)
	{
		int32 number = PlayerRankings.Num() + 1;

		PlayerRankings.Add(InPlayerController);
		number++;
		CLog::Print(InPlayerController);
		CLog::Print(number);

	}
}
