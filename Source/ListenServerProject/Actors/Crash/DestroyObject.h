#pragma once

#include "CoreMinimal.h"
#include "Characters/CrashCharacter.h"
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

	UPROPERTY(EditAnywhere)
	float life;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Shot();

	void ClearActors();

private:
	ACrashCharacter* Owner;

	TSet<AActor*> OverlappedActors;


};

