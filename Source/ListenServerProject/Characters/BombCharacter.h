#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "BombCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, AActor*, InAttackCuaser, class ACharacter*, InOther);

UCLASS()
class LISTENSERVERPROJECT_API ABombCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

public:
	ABombCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Attack_Montage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Dead_Montage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* TopDownSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCamera;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* HandSphere;

public:
	FAttachmentBeginOverlap OnAttachmentBeginOverlap;

	class ABombCharacter* PlayerCharacter;

	class ABomb* Bomb;

public:
	void Action() override;

	void Attack();

	UFUNCTION(Server, Reliable)
	void ServerAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttack();

	// 서버에서 폭탄을 생성
	UFUNCTION(Server, Reliable)
	void ServerSpawnBomb(TSubclassOf<class ABomb> BombSpawn);

	// 모든 클라이언트에게 폭탄 생성 및 부착 정보 전파
	UFUNCTION(NetMulticast, Reliable)
	void MultiSpawnBomb(ABomb* SpawnBomb);

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackSuccess(ACharacter* Attacker, ACharacter* HitActor);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(Replicated)
	bool bBombReplicate;

	UPROPERTY(Replicated)
	bool bBombReplicateMovement;

	UPROPERTY(Replicated)
	FVector BombLocation;

	UPROPERTY(Replicated)
	bool bBomb = false;

	bool bAttack = false;

private:
	void EnableCollision();
	void DisableCollision();

	void BombExplosion();

	void ResetBomb();

	void Dead();

	FTimerHandle CollisionTimerHandle;

	FTimerHandle BombTimerHandle;

	FTimerHandle DeadTimerHandle;

};

