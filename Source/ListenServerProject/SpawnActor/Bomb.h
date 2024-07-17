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

public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Bomb")
	bool bBombReplicate;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Bomb")
	bool bBombReplicateMovement;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateBombLocation, BlueprintReadOnly, Category = "Bomb")
	FVector BombLocation;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_UpdateBombLocation();

	UFUNCTION(NetMulticast, Reliable)
	void Explosion();

	void DestroyBomb();
};
