#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/WeaponComponent.h"
#include "AnimInstance_DefaultCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UAnimInstance_DefaultCharacter : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	float Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	bool bFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	bool bCanMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	bool bUseControlYaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	bool bIsCrouching;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WeaponType")
	EWeaponType WeaponType = EWeaponType::Gun;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadWrite)
	class ADefaultCharacter* OwnerCharacter;

private:
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

private:
	FRotator PrevRotation;
};
