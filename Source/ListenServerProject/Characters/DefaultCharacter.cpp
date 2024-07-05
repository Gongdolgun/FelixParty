#include "Characters/DefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/MoveComponent.h"
#include "Components/WeaponComponent.h"
#include "Global.h"

ADefaultCharacter::ADefaultCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	Helpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	Helpers::CreateActorComponent<UMoveComponent>(this, &MoveComponent, "MoveComponent");
	Helpers::CreateActorComponent<UWeaponComponent>(this, &WeaponComponent, "Weapon");

	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->TargetArmLength = 400;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

}

void ADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, MoveComponent, &UMoveComponent::Move);

<<<<<<< HEAD
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, Move, &UMoveComponent::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, Move, &UMoveComponent::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, Move, &UMoveComponent::StopJump);

		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ADefaultCharacter::HandleEquipAction);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, Weapon, &UWeaponComponent::Begin_Fire);
=======
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, MoveComponent, &UMoveComponent::Look);
>>>>>>> test

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, MoveComponent, &UMoveComponent::StartJump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, MoveComponent, &UMoveComponent::StopJump);
		
		EnhancedInputComponent->BindAction(IA_Action, ETriggerEvent::Triggered, this, &ThisClass::Action);
	}
}

<<<<<<< HEAD
void ADefaultCharacter::HandleEquipAction(const FInputActionValue& Value)
{
	// Value의 실제 자료형과 값 출력
	UE_LOG(LogTemp, Warning, TEXT("Received Value: %s"), *Value.ToString());

	// float로 변환하고 int32로 캐스팅
	float InputValue = Value.Get<float>();

	UE_LOG(LogTemp, Warning, TEXT("Input Value: %f"), InputValue);

	//int32 WeaponIndex = static_cast<int32>(InputValue);
	int32 WeaponIndex = FMath::RoundToInt(InputValue);

	UE_LOG(LogTemp, Warning, TEXT("Weapon Index: %f"), WeaponIndex);

	if (Weapon)
	{
		Weapon->SelectWeapon(WeaponIndex);
	}

}


=======
void ADefaultCharacter::Action()
{

}
>>>>>>> test
