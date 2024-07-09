#pragma once

#include "CoreMinimal.h"
#include "GameModes/DefaultGameMode.h"
#include "BombGameMode.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ABombGameMode : public ADefaultGameMode
{
	GENERATED_BODY()

public:
	ABombGameMode();

protected:
	void BeginPlay() override;

};
