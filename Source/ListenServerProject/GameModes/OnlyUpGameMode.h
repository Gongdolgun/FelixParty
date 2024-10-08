#pragma once

#include "CoreMinimal.h"
#include "GameModes/DefaultGameMode.h"
#include "OnlyUpGameMode.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AOnlyUpGameMode : public ADefaultGameMode
{
	GENERATED_BODY()

public:
	AOnlyUpGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

};
