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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* TopDownSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCamera;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* HandSphere;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABomb> BombClass;

public:
	FAttachmentBeginOverlap OnAttachmentBeginOverlap;

	class ACharacter* PlayerCharacter;

public:
	void Action() override;

	UFUNCTION()

	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	FTimerHandle BombTimerHandle;

	bool bBomb = false;
};
