#pragma once

#include "CoreMinimal.h"
#include "Characters/BombCharacter.h"
#include "GameModes/DefaultGameMode.h"
#include "BombGameMode.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ABombGameMode : public ADefaultGameMode
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABomb> BombClass;

	TArray<ADefaultController*> SurvivedControllers;

public:
	UPROPERTY(BlueprintReadOnly)
	ABomb* SpawnedBomb;

private:
	UFUNCTION()
	void CreateBombSpawn(EGameStateType InPrevGameType, EGameStateType InNewGameType);

public:
	void OnPlayerDead(ABombCharacter* DeadPlayer);

	void SpawnBomb();

	void HitAttachBomb(ABombCharacter* InCharacter);

	void SomeoneDead(ADefaultController* InController);

	FTimerHandle SpawnBombTimerHandle;

};
