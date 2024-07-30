#pragma once

#include "CoreMinimal.h"
#include "Characters/BombCharacter.h"
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

	void OnPostLogin(AController* NewPlayer) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABomb> BombClass;

public:
	void RandomSpawn();

	void OnPlayerDead(ABombCharacter* DeadPlayer);

	void CheckGameEnd();

private:
	TArray<class ADefaultController*> PlayerControllers;

public:
	AController* BombHolderController;

public:
	void SetHolderController(ADefaultController* NewController);

public:
	FTimerHandle BombTimerHandle;

	FTimerHandle SpawnCharacterTimerHandle;

};
