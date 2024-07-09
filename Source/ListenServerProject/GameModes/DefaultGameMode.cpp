#include "GameModes/DefaultGameMode.h"
#include "Global.h"

APawn* ADefaultGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	return Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
}
