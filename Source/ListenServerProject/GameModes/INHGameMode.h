#pragma once

#include "CoreMinimal.h"
#include "GameModes/DefaultGameMode.h"
#include "INHGameMode.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AINHGameMode : public ADefaultGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "GhostClass")
	TSubclassOf<APawn> Ghost;

public:
	void SetGhostMode(AController* InController);
};