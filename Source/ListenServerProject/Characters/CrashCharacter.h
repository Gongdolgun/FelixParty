#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "CrashCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ACrashCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

public:
	ACrashCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleDefaultsOnly)
	class UZoomComponent* Zoom;

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Attack_Montage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Throw_Montage;

private:
	UPROPERTY(EditAnywhere, Category = "DestroyObject")
	TSubclassOf<class ADestroyObject> DestroyObjectClass;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_SubAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Zoom;

public:
	void Action() override;

	void SubAction();

	UFUNCTION(NetMulticast, Reliable)
	void Attack_NMC();

	UFUNCTION(Server, Reliable)
	void Attack_Server();

	UFUNCTION(NetMulticast, Reliable)
	void ThrowAction_NMC();

	UFUNCTION(Server, Reliable)
	void ThrowAction_Server();

	UFUNCTION(Server, Reliable)
	void SpawnDestoryObject_Server();

	void PlayMontage(UAnimMontage* Montage);

	void SetZooming(const FInputActionValue& Value);

private:
	int SpawnCount;

};
