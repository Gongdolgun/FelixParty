#include "Components/MoveComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "ListenServerProjectCharacter.h"

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

void UMoveComponent::MoveForward(float InAxis)
{
	FRotator rotator = FRotator(0, Character->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	direction = FVector::XAxisVector;

	Character->AddMovementInput(direction, InAxis);
}

void UMoveComponent::MoveRight(float InAxis)
{
	FRotator rotator = FRotator(0, Character->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	direction = FVector::YAxisVector;

	Character->AddMovementInput(direction, InAxis);
}
