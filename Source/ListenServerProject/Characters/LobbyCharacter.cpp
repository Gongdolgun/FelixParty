#include "Characters/LobbyCharacter.h"
#include "Global.h"
#include "Controllers/LobbyController.h"
#include "GameModes/LobbyGameMode.h"
#include "GameState/LobbyGameState.h"

ALobbyCharacter::ALobbyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	Helpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	Helpers::CreateActorComponent<UMoveComponent>(this, &MoveComponent, "MoveComponent");

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

void ALobbyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//UpdatePlayer_Server();
}

void ALobbyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALobbyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, MoveComponent, &UMoveComponent::Move);

		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, MoveComponent, &UMoveComponent::Look);

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ThisClass::Jump);
	}
}

void ALobbyCharacter::ChangeMaterial(FColor InColor)
{
	if (HasAuthority())
		ChangeMaterial_NMC(InColor);

	else
		ChangeMaterial_Server(InColor);
}

void ALobbyCharacter::UpdatePlayer_Server_Implementation()
{
	if(ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode()))
		LobbyGameMode->UpdatePlayerMaterial();
}

void ALobbyCharacter::ChangeMaterial_Server_Implementation(FColor InColor)
{
	ChangeMaterial_NMC(InColor);
}

void ALobbyCharacter::ChangeMaterial_NMC_Implementation(FColor InColor)
{
	int32 MaterialCount = GetMesh()->GetNumMaterials();
	for(int32 i = 0; i < MaterialCount; i++)
	{
		UMaterialInstanceDynamic* MaterialInstance = Cast<UMaterialInstanceDynamic>(GetMesh()->GetMaterial(i));
		if (!MaterialInstance)
			MaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(i);

		if (MaterialInstance)
		{
			if(MaterialInstance)
				MaterialInstance->SetVectorParameterValue(FName("Tint"), InColor);
		}
	}
}

void ALobbyCharacter::AddSelectedColor_Implementation(const FString& InColor)
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(LobbyGameMode->GetGameState<ALobbyGameState>());
		if(LobbyGameState)
		{
			if (!LobbyGameState->SelectedColors.Contains(InColor))
				LobbyGameState->SelectedColors.Add(InColor);
		}
	}
}

void ALobbyCharacter::RemoveSelectedColor_Implementation(const FString& InColor)
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(LobbyGameMode->GetGameState<ALobbyGameState>());
		if (LobbyGameState)
		{
			if (LobbyGameState->SelectedColors.Contains(InColor))
				LobbyGameState->SelectedColors.Remove(InColor);
		}
	}
}