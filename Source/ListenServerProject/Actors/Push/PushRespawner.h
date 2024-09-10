#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PushRespawner.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API APushRespawner : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* Collision;

public:	
	APushRespawner();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	FVector GetSpawnCollisionRandomPoint();

private:
	UPROPERTY(Replicated)
	FVector SpawnLocation;
};
