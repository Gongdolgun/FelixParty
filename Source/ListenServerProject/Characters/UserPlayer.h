#pragma once

#include "CoreMinimal.h"
#include "ListenServerProjectCharacter.h"
#include "UserPlayer.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AUserPlayer : public AListenServerProjectCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

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

	UPROPERTY(VisibleAnywhere)
	UInputAction* FireAction;

private:
	UPROPERTY(VisibleAnywhere)
	class UMoveComponent* Move;

	UPROPERTY(VisibleAnywhere)
	class UWeaponComponent* Weapon;

public:
	AUserPlayer();

protected:
	void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

};
