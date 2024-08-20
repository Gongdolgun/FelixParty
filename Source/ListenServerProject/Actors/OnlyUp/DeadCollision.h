#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeadCollision.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADeadCollision : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Root")
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* Collision;

public:	
	ADeadCollision();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, Category = "Actor")
	TSubclassOf<class AActor> SpawnLocation;

	TArray<AActor*> SpawnActors;

};
