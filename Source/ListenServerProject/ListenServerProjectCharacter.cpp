#include "ListenServerProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Global.h"
#include "Utilites/Helpers.h"
#include "Utilites/CLog.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/MoveComponent.h"
#include "Components/WeaponComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AListenServerProjectCharacter::AListenServerProjectCharacter()
{
	

}

void AListenServerProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	//if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	//{
	//	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	//	{
	//		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	//	}
	//}
}

void AListenServerProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

