#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Structures.h"
#include "DestroyObject.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADestroyObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ADestroyObject();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Particle;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* Projectile;


private:
	UPROPERTY(EditAnywhere, Category = "HitData")
	FHitData HitData;

};
