#pragma once

#include "CoreMinimal.h"
#include "Actors/Spawner.h"
#include "Spawner_Weapons.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ASpawner_Weapons : public ASpawner
{
	GENERATED_BODY()

public:
	ASpawner_Weapons();
	virtual void BeginPlay() override;

private:
	FTimerHandle timer;
	bool isDestroy;

private:
	void SpawnActor();

public:
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	UPROPERTY(EditAnywhere)
	float SpawnTimeRate = 5.f;
};