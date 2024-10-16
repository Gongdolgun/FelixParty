#pragma once

#include "CoreMinimal.h"
#include "Actors/Spawner.h"
#include "Spawner_TotalCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ASpawner_TotalCharacter : public ASpawner
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void SpawnActor() override;
};
