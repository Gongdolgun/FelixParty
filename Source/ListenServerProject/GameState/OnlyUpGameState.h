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

public:
	UPROPERTY(VisibleAnywhere, Replicated)
	TArray<class APlayerController*> PlayerRanking;

public:
	int32 PlayerRank(APlayerController* InController);
	const TArray<int32> RankScores = { 50, 30, 20, 10 };


};
