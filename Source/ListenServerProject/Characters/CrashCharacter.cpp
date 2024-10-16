#include "Characters/CrashCharacter.h"
#include "EnhancedPlayerInput.h"
#include "Global.h"
#include "Components/ZoomComponent.h"
#include "Components/MoveComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACrashCharacter::ACrashCharacter()
{
	Helpers::CreateActorComponent<UZoomComponent>(this, &Zoom, "Zoom");
}

void ACrashCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACrashCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACrashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Action, ETriggerEvent::Started, this, &ACrashCharacter::Action);
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Started, this, &ACrashCharacter::SetZooming);
	}
}

void ACrashCharacter::Action()
{
	Super::Action();

}

void ACrashCharacter::SetZooming(const FInputActionValue& Value)
{
	float InValue = Value.Get<float>(); 

	InValue = -InValue;

	Zoom->SetZoomValue(InValue);
}
