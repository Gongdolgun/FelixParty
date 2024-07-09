#pragma once

#include "CoreMinimal.h"
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

public:
    UPROPERTY(EditAnywhere)
    TSubclassOf<class ACharacter> CharacterClass;

    UPROPERTY(EditAnywhere)
    TArray<TSoftObjectPtr<AActor>> SpawnPoints;

    UPROPERTY(EditAnywhere)
    int32 NumberCharacters = 5;

private:
    void SpawnCharacters();


};
