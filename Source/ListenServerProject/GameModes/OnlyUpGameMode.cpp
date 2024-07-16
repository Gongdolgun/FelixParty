#include "GameModes/OnlyUpGameMode.h"
#include "Global.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/OnlyUpHUD.h"

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
