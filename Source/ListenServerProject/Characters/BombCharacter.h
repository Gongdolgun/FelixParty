#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "SpawnActor/TargetDecal.h"
#include "BombCharacter.generated.h"

enum class EActionState
{
	Idle,
	InAction,
	Dead
};


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

//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
//	ACameraActor* CameraActor;

private:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Attack_Montage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Wall_Montage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Restraint_Montage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Dead_Montage;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* HandSphere;

private:
	UPROPERTY(EditAnywhere, Category = "Wall")
	TSubclassOf<class AWall> WallClass;

	UPROPERTY(EditAnywhere, Category = "Restraint")
	TSubclassOf<class ARestraint> RestraintClass;

	UPROPERTY(EditDefaultsOnly, Category = "Wall")
	TSubclassOf<class ADecalActor> DecalClass;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_SubAction;

	UPROPERTY(EditAnywhere, Category = "Wall")
	float Rate;

public:
	FAttachmentBeginOverlap OnAttachmentBeginOverlap;

	class ABombCharacter* PlayerCharacter;

	class ABomb* Bomb;

	EActionState CurrentActionState = EActionState::Idle;

	ATargetDecal* TargetDecal;

	//ACameraActor* TopDownCameraActor;

public:
	void Action() override;

	void HandleAction();

	void Attack();

	UFUNCTION(Server, Reliable)
	void ServerAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttack();

	UFUNCTION(Server, Reliable)
	void ServerPlayWall();

	UFUNCTION(NetMulticast, Reliable)
	void MultiPlayWall();

	UFUNCTION(Server, Reliable)
	void ServerSpawnWall();

	UFUNCTION(Server, Reliable)
	void ServerPlayRestraint();

	UFUNCTION(NetMulticast, Reliable)
	void MultiPlayRestraint();

	UFUNCTION(Server, Reliable)
	void ServerSpawnRestraint();

	// �������� ��ź�� ����
	UFUNCTION(Server, Reliable)
	void ServerSpawnBomb(TSubclassOf<class ABomb> BombSpawn);

	// ��� Ŭ���̾�Ʈ���� ��ź ���� �� ���� ���� ����
	UFUNCTION(NetMulticast, Reliable)
	void MultiSpawnBomb(ABomb* SpawnBomb);

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackSuccess(ACharacter* Attacker, ACharacter* HitActor);

	void ShowDecal(FVector TargetLocation);

	void PlaceWall();

	void SetActionState(EActionState NewState);

	bool IsInAction() const;

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

	UPROPERTY(Replicated)
	bool bIsDead = false;

	bool bAttack = false;

	bool bIsSpawningRestraint = false;

	bool bIsDecal = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* NewCountdownSound;

private:
	void EnableCollision();
	void DisableCollision();

	void BombExplosion();

	void ResetBomb();

	void PlayDead();

	void Dead();

	UFUNCTION(NetMulticast, Reliable)
	void MultiDead();

	UFUNCTION(NetMulticast, Reliable)
	void MultiDestroyCharacter();

	FTimerHandle CollisionTimerHandle;

	FTimerHandle BombTimerHandle;

	FTimerHandle BombParticleHandle;

	FTimerHandle DeadTimerHandle;

	FTimerHandle ResetSpawnFlagHandle;

public:
	bool IsAlive() const { return !bIsDead; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseMovementSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BombMovementSpeed = 1.5f;

	UFUNCTION(BlueprintCallable)
	float GetCurrentMovementSpeed() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoolTime")
	float WallCoolTime = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoolTime")
	float RestraintCoolTime = 30.0f;

	float LastWallSpawnTime;

	float LastRestraintSpawnTime;
};


