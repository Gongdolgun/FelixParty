#pragma once

#include "CoreMinimal.h"
#include "GameState/DefaultGameState.h"
#include "OnlyUpGameState.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AOnlyUpGameState : public ADefaultGameState
{
	GENERATED_BODY()

public:
	AOnlyUpGameState();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(VisibleAnywhere, Replicated)
	TArray<class APlayerController*> PlayerRanking;

public:
	void PlayerRank(APlayerController* InController);

};
