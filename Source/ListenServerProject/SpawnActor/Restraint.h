#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Restraint.generated.h"

class ABombCharacter;
UCLASS()
class LISTENSERVERPROJECT_API ARestraint : public AActor
{
	GENERATED_BODY()
	
public:	
	ARestraint();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* Capsule;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* Niagara;

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleDefaultsOnly)
	class UProjectileMovementComponent* Projectile;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DisableMovement(ABombCharacter* TargetCharacter);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	class ABombCharacter* PlayerCharacter;

	FTimerHandle MovementTimerHandle;

};
