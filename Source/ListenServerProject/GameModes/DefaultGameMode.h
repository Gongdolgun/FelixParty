#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Misc/Structures.h"
#include "DefaultGameMode.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADefaultGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void BeginPlay() override;

public:
	TArray<class ADefaultController*> ConnectedPlayers;

	UPROPERTY(EditAnywhere)
	bool IsDebugging = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ATotalScoreCharacter> TotalScoreCharacterClass;

public:
	void UpdatePlayer();

public:
	UFUNCTION()
	void OnGameStateTypeChanged(EGameStateType InPrevGameType, EGameStateType InNewGameType);
};