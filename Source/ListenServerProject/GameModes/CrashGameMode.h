#pragma once

#include "CoreMinimal.h"
#include "GameModes/DefaultGameMode.h"
#include "CrashGameMode.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ACrashGameMode : public ADefaultGameMode
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

};
