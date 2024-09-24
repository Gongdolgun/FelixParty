#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "INHCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AINHCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

public:
	AINHCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Hit(AActor* InActor, const FHitData& InHitData) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

	virtual void OnCollision() override;
	virtual void OffCollision() override;

protected:
	virtual void Action() override;
	virtual void End_Action() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditAnywhere)
	class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	class UStateComponent* StateComponent;

public:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Run;

	UPROPERTY(BlueprintReadWrite)
	bool isRun;

	UPROPERTY(EditAnywhere, Category = "PunchMontage")
	UAnimMontage* PunchMontage;

	UPROPERTY(EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShakeBase;

private:
	UPROPERTY(Replicated)
	float CurrentSpeed;

	UPROPERTY(Replicated)
	bool canRun = false;
	

public:
	UFUNCTION(Server, Reliable)
	void SetSpeed_Server(bool InCanRun);

	UFUNCTION(BlueprintCallable)
	void SetSpeed(bool InIsRun);

	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void PlayAnimMontage_Server(UAnimMontage* InAnimMontage);

	UFUNCTION(NetMulticast, Reliable)
	void PlayAnimMontage_NMC(UAnimMontage* InAnimMontage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeadMaterialEvent();
	void DeadMaterialEvent_Implementation();

private:
	UFUNCTION(NetMulticast, Reliable)
	void Dead_NMC(FVector InImpulseDirection);

	void SetGhostMode();
};