#include "Components/MoveComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Global.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameInstances/OnlineGameInstance.h"

UMoveComponent::UMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACharacter>(GetOwner());

	if (Owner)
		GameInstance = Cast<UOnlineGameInstance>(UGameplayStatics::GetGameInstance(Owner->GetWorld()));
}

void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UMoveComponent::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Owner != nullptr && CanMove)
	{
		const FRotator Rotation = Owner->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		Owner->AddMovementInput(ForwardDirection, MovementVector.Y);
		Owner->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void UMoveComponent::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	LookAxisVector.X *= (GameInstance->Instance_Settings.MouseSenX);
	LookAxisVector.Y *= (GameInstance->Instance_Settings.MouseSenY);

	if (Owner != nullptr && CanMove)
	{
		float YawInput = LookAxisVector.X * (GameInstance->Instance_Settings.bInvertX ? -1.0f : 1.0f);
		float PitchInput = LookAxisVector.Y * (GameInstance->Instance_Settings.bInvertY ? -1.0f : 1.0f);

		Owner->AddControllerYawInput(YawInput);
		Owner->AddControllerPitchInput(PitchInput);
	}
}

void UMoveComponent::Jump()
{
	if(Owner != nullptr && CanMove)
		Owner->Jump();
}

void UMoveComponent::DoCrouch()
{
	if (Owner->GetMovementComponent()->IsCrouching())
	{
		UnCrouch();
	}

	else
	{
		Crouch();
	}
}

void UMoveComponent::Crouch()
{
	if (Owner != nullptr && CanMove && Owner->CanJump() == true)
	{
		Owner->Crouch();
		EnableControlRotation();
	}
		
}

void UMoveComponent::UnCrouch()
{
	if (Owner != nullptr && CanMove)
	{
		Owner->UnCrouch();
		DisableControlRotation();
	}
}

void UMoveComponent::EnableControlRotation()
{
	Owner->bUseControllerRotationYaw = true;
	Owner->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UMoveComponent::DisableControlRotation()
{
	Owner->bUseControllerRotationYaw = false;
	Owner->GetCharacterMovement()->bOrientRotationToMovement = true;
}

