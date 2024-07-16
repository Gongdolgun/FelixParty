#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<AActor> SpawnActorClass;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FVector SpawnLocation = FVector(0, 0, 60);

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool isRotate;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FRotator RotateRate = FRotator(0, 180, 0);

protected:
	AActor* SpawnedActor;

public:
	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void CreateRotatingMovementComponent();
};
