#pragma once

#include "CoreMinimal.h"
#include "GameState/DefaultGameState.h"
#include "SpawnActor/Restraint.h"
#include "BombGameState.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ABombGameState : public ADefaultGameState
{
	GENERATED_BODY()

public:
	ABombGameState();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
