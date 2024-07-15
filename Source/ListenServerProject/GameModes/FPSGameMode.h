#pragma once

#include "CoreMinimal.h"
#include "GameModes/DefaultGameMode.h"
#include "FPSGameMode.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AFPSGameMode : public ADefaultGameMode
{
	GENERATED_BODY()

public:
	void RespawnPlayer(AController* controller);
};
