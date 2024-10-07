#pragma once

#include "CoreMinimal.h"
#include "GameState/DefaultGameState.h"
#include "PushGameState.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API APushGameState : public ADefaultGameState
{
	GENERATED_BODY()

public:
	APushGameState();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
};
