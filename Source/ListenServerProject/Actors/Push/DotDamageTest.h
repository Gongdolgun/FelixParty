#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Structures.h"
#include "DotDamageTest.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADotDamageTest : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* BoxCollision;

public:	
	ADotDamageTest();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ApplyDamage(class ADefaultCharacter* InCharacter);

private:
	float DotDamage = 5.0f;
	float DotInterval = 0.5f;

	FTimerHandle DotTimerHandle;
	FHitData HitData;
};


