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
#include "Controllers/DefaultController.h"
#include "GameModes/DefaultGameMode.h"

ADefaultCharacter::ADefaultCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	Helpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	Helpers::CreateActorComponent<UMoveComponent>(this, &MoveComponent, "MoveComponent");
	Helpers::CreateActorComponent<UWeaponComponent>(this, &WeaponComponent, "WeaponComponent");

	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->TargetArmLength = 270;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SocketOffset = FVector(60, 0, 0);

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

	UpdatePlayer_Server();
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

		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, MoveComponent, &UMoveComponent::Look);

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ThisClass::Jump);
		
		EnhancedInputComponent->BindAction(IA_Action, ETriggerEvent::Started, this, &ThisClass::Action);
	}
}

void ADefaultCharacter::Hit(AActor* InActor, const FHitData& InHitData)
{
}

void ADefaultCharacter::Action()
{
}

void ADefaultCharacter::UpdatePlayer_Server_Implementation()
{
	if (ADefaultGameMode* DefaultGameMode = Cast<ADefaultGameMode>(GetWorld()->GetAuthGameMode()))
		DefaultGameMode->UpdatePlayer();
}

void ADefaultCharacter::ChangeMaterial()
{
	if (HasAuthority())
		if (ADefaultController* DefaultController = Cast<ADefaultController>(Controller))
			ChangeMaterial_NMC(DefaultController->MyMaterials);

	else
		ChangeMaterial_Server();
}

void ADefaultCharacter::ChangeMaterial_Server_Implementation()
{
	if (ADefaultController* DefaultController = Cast<ADefaultController>(Controller))
		ChangeMaterial_NMC(DefaultController->MyMaterials);
}

void ADefaultCharacter::ChangeMaterial_NMC_Implementation(const TArray<UMaterialInterface*>& InMaterials)
{
	if (InMaterials.Num() > 0)
	{
		for (int i = 0; i < InMaterials.Num(); i++)
		{
			GetMesh()->SetMaterial(i, InMaterials[i]);
		}
	}
}
