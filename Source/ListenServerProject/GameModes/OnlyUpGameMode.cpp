#include "GameModes/OnlyUpGameMode.h"
#include "Global.h"
#include "GameState/OnlyUpGameState.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/DefaultHUD.h"

AOnlyUpGameMode::AOnlyUpGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOnlyUpGameMode::BeginPlay()
{
	Super::BeginPlay();

	
}

void AOnlyUpGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}


