#include "Components/MoveComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Global.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Character.h"

UMoveComponent::UMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ACharacter>(GetOwner());
}

void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UMoveComponent::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Character != nullptr)
	{
		const FRotator Rotation = Character->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		Character->AddMovementInput(ForwardDirection, MovementVector.Y);
		Character->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void UMoveComponent::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Character != nullptr)
	{
		Character->AddControllerYawInput(LookAxisVector.X);
		Character->AddControllerPitchInput(LookAxisVector.Y);
	}
}
