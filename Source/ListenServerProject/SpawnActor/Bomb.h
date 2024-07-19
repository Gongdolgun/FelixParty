#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	ABomb();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Particle;

	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere)
	UAudioComponent* Audio;

	UPROPERTY(EditAnywhere)
	USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
	USoundBase* CountdownSound;

public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Bomb")
	bool bBombReplicate;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Bomb")
	bool bBombReplicateMovement;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateBombLocation, BlueprintReadOnly, Category = "Bomb")
	FVector BombLocation;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Bomb")
	float ElapseTime;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_UpdateBombLocation();

	UFUNCTION(NetMulticast, Reliable)
	void Explosion();

	void DestroyBomb();

	void StartCountdown();

	UFUNCTION(NetMulticast, Reliable)
	void MultiPlayCountdownSound();

	UFUNCTION(NetMulticast, Reliable)
	void MultiPlayExplosionSound();

	void UpDateSound(float DeltaTime);

private:
	FTimerHandle CountdownTimerHandle;
	FTimerHandle TimerHandle_DestroyBomb;

	float TotalCountdownTime;

};
