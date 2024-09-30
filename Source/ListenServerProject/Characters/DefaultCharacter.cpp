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
#include "Blueprint/UserWidget.h"
#include "Controllers/DefaultController.h"
#include "GameModes/DefaultGameMode.h"
#include "GameState/DefaultGameState.h"
#include "Net/UnrealNetwork.h"

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

	GetCharacterMovement()->GravityScale = 2.0f;
}

void ADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	HP = MaxHP;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	ADefaultGameState* DefaultGameState = GetWorld()->GetGameState<ADefaultGameState>();
	if (DefaultGameState != nullptr)
		DefaultGameState->OnGameStateTypeChanged.AddDynamic(this, &ADefaultCharacter::PlayMaterialEventOnGameStart);

	if(IsMaterialChange)
		UpdatePlayer_Server();

	// Option
	if (SelectOptionWidget)
	{
		OptionWidget = CreateWidget<UUserWidget>(GetWorld(), SelectOptionWidget);
		
	}

	// Emote
	if (SelectEmoteWidget)
	{
		EmoteWidget = CreateWidget<UUserWidget>(GetWorld(), SelectEmoteWidget);
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

		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, MoveComponent, &UMoveComponent::Look);

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, MoveComponent, &UMoveComponent::Jump);
		
		EnhancedInputComponent->BindAction(IA_Action, ETriggerEvent::Started, this, &ThisClass::Action);
		EnhancedInputComponent->BindAction(IA_Action, ETriggerEvent::Completed, this, &ThisClass::End_Action);

		EnhancedInputComponent->BindAction(IA_Option, ETriggerEvent::Started, this, &ThisClass::OptionMenu);

		EnhancedInputComponent->BindAction(IA_Emote, ETriggerEvent::Started, this, &ThisClass::EmoteMenuOn);
		EnhancedInputComponent->BindAction(IA_Emote, ETriggerEvent::Completed, this, &ThisClass::EmoteMenuOff);
	}
}

void ADefaultCharacter::OnCollision()
{
}

void ADefaultCharacter::OffCollision()
{
}

void ADefaultCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, HP);
}

void ADefaultCharacter::Hit(AActor* InActor, const FHitData& InHitData)
{
}

void ADefaultCharacter::Action()
{
}

void ADefaultCharacter::End_Action()
{
}

void ADefaultCharacter::UpdatePlayer_Server_Implementation()
{
	if (ADefaultGameMode* DefaultGameMode = Cast<ADefaultGameMode>(GetWorld()->GetAuthGameMode()))
		DefaultGameMode->UpdatePlayer();
}

void ADefaultCharacter::PlayMaterialEventOnGameStart(EGameStateType InPrevGameType, EGameStateType InNewGameType)
{
	// 게임 시작 시 머티리얼 이벤트 발생
	if (InNewGameType == EGameStateType::GameStart)
		PlayMaterialEvent();
}

void ADefaultCharacter::PlayMaterialEvent_Implementation()
{
}

void ADefaultCharacter::ChangeMaterial(FColor InColor)
{
	if (HasAuthority())
		ChangeMaterial_NMC(InColor);

	else
		ChangeMaterial_Server(InColor);
}



void ADefaultCharacter::ChangeMaterial_Server_Implementation(FColor InColor)
{
	ChangeMaterial_NMC(InColor);
}

void ADefaultCharacter::ChangeMaterial_NMC_Implementation(FColor InColor)
{
	// Mesh Material 변경
	int32 MaterialCount = GetMesh()->GetNumMaterials();
	for (int32 i = 0; i < MaterialCount; i++)
	{
		UMaterialInstanceDynamic* MaterialInstance = Cast<UMaterialInstanceDynamic>(GetMesh()->GetMaterial(i));
		if (!MaterialInstance)
			MaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(i);

		if (MaterialInstance)
		{
			MyMaterials.Add(MaterialInstance);
			MaterialInstance->SetVectorParameterValue(FName("Tint"), InColor);
		}
	}

	if(OverlayMaterial != nullptr)
	{
		UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(OverlayMaterial, this);

		if(MaterialInstance)
		{
			MaterialInstance->SetVectorParameterValue(FName("Color"), FLinearColor(InColor));
			GetMesh()->SetOverlayMaterial(MaterialInstance);
		}
	}
}

void ADefaultCharacter::OptionMenu()
{
	if (OptionWidget)
	{
		OptionWidget->AddToViewport();

		ADefaultController* controller = Cast<ADefaultController>(GetController());
		if (controller)
		{
			OptionWidget->SetFocus();
			controller->SetShowMouseCursor(true);
			controller->SetInputMode(FInputModeGameAndUI());

		}
	}
}

void ADefaultCharacter::EmoteMenuOn()
{
	if (EmoteWidget)
	{
		if (!EmoteWidget->IsInViewport())
		{
			EmoteWidget->AddToViewport();

			ADefaultController* controller = Cast<ADefaultController>(GetController());
			if (controller)
			{
				EmoteWidget->SetFocus();
				controller->SetShowMouseCursor(true);
				controller->SetInputMode(FInputModeGameAndUI());
			}
		}
	}
}

void ADefaultCharacter::EmoteMenuOff()
{
	if (EmoteWidget)
	{
		//if (EmoteWidget->IsInViewport())
		//{
		//	EmoteWidget->RemoveFromParent();
		//	
		//	ADefaultController* controller = Cast<ADefaultController>(GetController());
		//	if (controller)
		//	{
		//		controller->SetShowMouseCursor(false);
		//		controller->SetInputMode(FInputModeGameOnly());
		//	
		//	}
		//}
		
	}
}

void ADefaultCharacter::PlayEmoteMontage_NMC_Implementation(UAnimMontage* InAnimMontage)
{
	PlayAnimMontage(InAnimMontage);
}

void ADefaultCharacter::PlayEmoteMontage_Server_Implementation(UAnimMontage* InAnimMontage)
{
	PlayEmoteMontage_NMC(InAnimMontage);
}

void ADefaultCharacter::PlayEmoteMontage(UAnimMontage* InAnimMontage)
{
	PlayEmoteMontage_Server(InAnimMontage);
}

