#include "Characters/CrashCharacter.h"
#include "EnhancedPlayerInput.h"
#include "Global.h"
#include "Components/ZoomComponent.h"
#include "Components/MoveComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/Crash/DestroyObject.h"
#include "Blueprint/UserWidget.h"
#include "Controllers/CrashController.h"
#include "GameState/DefaultGameState.h"

ACrashCharacter::ACrashCharacter()
{
	Helpers::CreateActorComponent<UZoomComponent>(this, &Zoom, "Zoom");

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed *= 1.5f;
		GetCharacterMovement()->JumpZVelocity *= 1.5f;
	}

	SpawnCount = 0;
}

void ACrashCharacter::BeginPlay()
{
	Super::BeginPlay();

	MoveComponent->EnableControlRotation();

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
		EnhancedInputComponent->BindAction(IA_SubAction, ETriggerEvent::Started, this, &ACrashCharacter::SubAction);
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Started, this, &ACrashCharacter::SetZooming);
	}
}

void ACrashCharacter::Action()
{
	Super::Action();

	if (GetCurrentMontage() == nullptr)
	{
		if (HasAuthority())
		{
			Attack_NMC();
		}
		else
		{
			Attack_Server();
		}
	}
}

void ACrashCharacter::SubAction()
{
	if (GetCurrentMontage() == nullptr)
	{
		if (HasAuthority())
		{
			ThrowAction_NMC();
		}
		else
		{
			ThrowAction_Server();
		}
	}
}

void ACrashCharacter::Attack_NMC_Implementation()
{
	PlayMontage(Attack_Montage);
}

void ACrashCharacter::Attack_Server_Implementation()
{
	Attack_NMC();
}

void ACrashCharacter::ThrowAction_NMC_Implementation()
{
	PlayMontage(Throw_Montage);
}

void ACrashCharacter::ThrowAction_Server_Implementation()
{
	ThrowAction_NMC();
}

void ACrashCharacter::SpawnDestoryObject_Server_Implementation()
{
	if (DestroyObjectClass && SpawnCount < 20)
	{
		FVector socketLocation = GetMesh()->GetSocketLocation(FName("Crash_R"));
		FRotator socketRotation = this->GetControlRotation();

		FActorSpawnParameters params;
		params.Owner = this;

		ADestroyObject* spawnObject = GetWorld()->SpawnActor<ADestroyObject>(DestroyObjectClass, socketLocation, socketRotation, params);

		if (spawnObject)
		{
			spawnObject->Shot();
			SpawnCount++;

			ACrashController* CrashController = Cast<ACrashController>(GetController());
			if (CrashController)
			{
				CrashController->CountUpdate_NMC(20 - SpawnCount, 20);
			}
		}
	}
}

void ACrashCharacter::PlayMontage(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (Montage && AnimInstance)
	{
		AnimInstance->Montage_Play(Montage);
	}
}

void ACrashCharacter::SetZooming(const FInputActionValue& Value)
{
	float InValue = Value.Get<float>();

	InValue = -InValue;

	Zoom->SetZoomValue(InValue);
}

