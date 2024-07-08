#include "Characters/AnimInstance_DefaultCharacter.h"
#include "Global.h"
#include "DefaultCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"


void UAnimInstance_DefaultCharacter::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ADefaultCharacter>(GetOwningActor());
	if (!OwnerCharacter) return;


}

void UAnimInstance_DefaultCharacter::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwnerCharacter)	return;

	Velocity = OwnerCharacter->GetVelocity();
	Speed = OwnerCharacter->GetVelocity().Size2D();
	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();
	bUseControlYaw = OwnerCharacter->bUseControllerRotationYaw;

	// 일정 속도 이상일 때만 움직일 수 있도록. 쓸만한지는 모르겠음 그냥 Unreal 5에서 구현되어있길래 만듦
	if (Speed > 3.0f)
	{
		FVector CurrentAcceleration = OwnerCharacter->GetCharacterMovement()->GetCurrentAcceleration();
		bCanMove = !(CurrentAcceleration.Equals(FVector::ZeroVector, 0.1f));
	}

	// AimOffset
	Pitch = OwnerCharacter->GetBaseAimRotation().Pitch;
	if (Pitch > 90.0f)
	{
		FVector2D InRange(270.0f, 360.0f);
		FVector2D OutRange(-90.0f, 0.0f);

		Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, Pitch);
	}
}

void UAnimInstance_DefaultCharacter::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}
