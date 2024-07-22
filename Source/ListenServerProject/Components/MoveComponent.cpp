#include "Components/MoveComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Global.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UMoveComponent::UMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACharacter>(GetOwner());
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

	if (Owner != nullptr && CanMove)
	{
		Owner->AddControllerYawInput(LookAxisVector.X);
		Owner->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void UMoveComponent::Jump()
{
	if(Owner != nullptr && CanMove)
		Owner->Jump();
}

void UMoveComponent::Run_Server_Implementation()
{
	
}

void UMoveComponent::Run()
{
	//Run_Server();

	if (Owner != nullptr && CanMove)
	{
		Owner->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}

void UMoveComponent::Walk_Server_Implementation()
{
	
}

void UMoveComponent::Walk()
{
	//Walk_Server();

	if (Owner != nullptr && CanMove)
	{
		Owner->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	}
}