#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Structures.h"
#include "FireBall.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AFireBall : public AActor
{
	GENERATED_BODY()
	
public:	
	AFireBall();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	void OnDestroy();

private:
	UPROPERTY(EditAnywhere, Category = "HitData")
	FHitData HitData;

	UPROPERTY(EditAnywhere, Category = "Particle")
	class UParticleSystem* Explosion;

	UPROPERTY(EditAnywhere, Category = "Value")
	float InterpSpeed;

public:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* Particle;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileComponent;

	UFUNCTION()
	void OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	ACharacter* OwnerCharacter;

	FVector Object_Velocity;
	FRotator Object_Rotation;

};
