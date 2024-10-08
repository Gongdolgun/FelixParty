#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Structures.h"
#include "ElectricField.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AElectricField : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Replicated)
	class UCapsuleComponent* RingCapsule;

	UPROPERTY(EditAnywhere, Replicated)
	class UStaticMeshComponent* RingMesh;

public:
	AElectricField();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ApplyDamage(class ADefaultCharacter* InCharacter);

	void SetFieldRadius(float InRadius);

private:
	float DotDamage = 5.0f;
	float DotInterval = 0.5f;

	FTimerHandle DotTimerHandle;
	FHitData HitData;

	TArray<class ADefaultCharacter*> OverlappedCharacters;

	class APushGameState* PushGameState;

	float GamePlayTime = 0.0f;
	float CapsuleRadius = 0.0f;

public:
	float CurrFieldRadius = 0.0f;

};


