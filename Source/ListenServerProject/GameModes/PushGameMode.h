#pragma once

#include "CoreMinimal.h"
#include "GameModes/DefaultGameMode.h"
#include "PushGameMode.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API APushGameMode : public ADefaultGameMode
{
	GENERATED_BODY()

public:
	APushGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void SpawnElectricField();


	void RespawnPlayer(AController* InController);

public:
	UPROPERTY(EditAnywhere, Category = "Ring")
	TSubclassOf<class AElectricField> ElectricFieldClass;

	
private:
	AElectricField* ElectricField;
	class APushGameState* PushGameState;

	float RespawnRadius = 0.0f;
};



