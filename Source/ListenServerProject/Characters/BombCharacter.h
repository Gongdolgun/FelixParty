#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "SpawnActor/TargetDecal.h"
#include "Widgets/PlayerSkillTime.h"
#include "Widgets/TargetAim.h"
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
	virtual void Hit(AActor* InActor, const FHitData& InHitData) override;
	virtual void OnCollision() override;
	virtual void OffCollision() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleDefaultsOnly)
	class UZoomComponent* Zoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TargetAimCamera;

public:
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* BombSphereComponent;

private:
	UPROPERTY(EditAnywhere, Category = "Wall")
	TSubclassOf<class AWall> WallClass;

	UPROPERTY(EditAnywhere, Category = "Restraint")
	TSubclassOf<class ARestraint> RestraintClass;

	UPROPERTY(EditDefaultsOnly, Category = "Wall")
	TSubclassOf<class ADecalActor> DecalClass;

	UPROPERTY(EditAnywhere, Category = "Restraint")
	TSubclassOf<class UTargetAim> TargetAimClass;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_SubAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Zoom;

public:
	FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	ADecalActor* TargetDecal;

public:
	void Action() override;
	void HandleAction();

	UFUNCTION(NetMulticast, Reliable)
	void DeadEvent_NMC();

	UFUNCTION(Server, Reliable)
	void ServerAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttack();

	UFUNCTION(Server, Reliable)
	void ServerSpawnWall(const FVector& Location, const FRotator& Rotation);

	UFUNCTION(Server, Reliable)
	void ServerSpawnRestraint(const FVector& Location, const FRotator& Rotation);

	UFUNCTION(Server, Reliable)
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetZooming(const FInputActionValue& Value);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bBomb = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAim = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* NewCountdownSound;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseMovementSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BombMovementSpeed = 1.5f;

	UFUNCTION(BlueprintCallable)
	void ChangeSpeed();

	UFUNCTION(Server, Reliable)
	void ChangeSpeed_Server(float InSpeed);

	UFUNCTION(NetMulticast, Reliable)
	void ChangeSpeed_NMC(float InSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWallCooltime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoolTime")
	float CurrentWallCoolTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoolTime")
	float RestraintCoolTime = 30.0f;

	void PlayMontage(UAnimMontage* Montage);

};