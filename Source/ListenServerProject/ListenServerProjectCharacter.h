#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ListenServerProjectCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AListenServerProjectCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	///////////////////////// Å°ÀÔ·Â
private: 
	UPROPERTY(VisibleAnywhere)
	UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere)
	UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere)
	UInputAction* JumpAction;

	UPROPERTY(VisibleAnywhere)
	UInputAction* EquipAction;

private:
	UPROPERTY(VisibleAnywhere)
	class UMoveComponent* Move;

	UPROPERTY(VisibleAnywhere)
	class UWeaponComponent* Weapon;

public:
	AListenServerProjectCharacter();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay();

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};