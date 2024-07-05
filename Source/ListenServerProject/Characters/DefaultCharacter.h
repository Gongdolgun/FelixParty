#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "Weapon/Weapon.h"
#include "DefaultCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADefaultCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void HandleEquipAction(const FInputActionValue& Value);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMoveComponent* MoveComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWeaponComponent* WeaponComponent;

private:
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere)
<<<<<<< HEAD
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere)
	UInputAction* FireAction;

=======
	UInputAction* IA_Action;

public:
	UFUNCTION(BlueprintCallable)
	virtual void Action();
>>>>>>> test
};
