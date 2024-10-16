#include "Characters/OnlyUpCharacter.h"

#include "EnhancedInputComponent.h"
#include "Components/ParkourComponent.h"
#include "Global.h"
#include "Components/ArrowComponent.h"
#include "Components/MoveComponent.h"
#include "Components/StateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/OnlyUpGameMode.h"
#include "Net/UnrealNetwork.h"

AOnlyUpCharacter::AOnlyUpCharacter()
{
	Helpers::CreateActorComponent<UParkourComponent>(this, &ParkourComponent, "Parkour");
	Helpers::CreateActorComponent<UStateComponent>(this, &StateComponent, "State");

	// 액터 자체 리플리케이션
	//SetReplicates(true);

	ParkourComponent->SetIsReplicated(true);
	StateComponent->SetIsReplicated(true);
	MoveComponent->SetIsReplicated(true);

	Helpers::CreateComponent<USceneComponent>(this, &ArrowGroup, "ArrowGroup", GetCapsuleComponent());
	Arrows.SetNum((int32)EParkourArrowType::Max);
	
	for (int32 i = 0; i < Arrows.Num(); i++)
	{
		FString name = StaticEnum<EParkourArrowType>()->GetNameStringByIndex(i);
		Helpers::CreateComponent<UArrowComponent>(this, &Arrows[i], FName(name), ArrowGroup);
	
		EParkourArrowType ArrowType = static_cast<EParkourArrowType>(i);
	
		switch (ArrowType)
		{
		case EParkourArrowType::Center:
			Arrows[i]->ArrowColor = FColor::Red;
			Arrows[i]->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
			break;
	
		case EParkourArrowType::Left:
			Arrows[i]->ArrowColor = FColor::Blue;
			Arrows[i]->SetRelativeLocation(FVector(0.0f, -20.0f, 20.0f));
			break;
	
		case EParkourArrowType::Right:
			Arrows[i]->ArrowColor = FColor::Green;
			Arrows[i]->SetRelativeLocation(FVector(0.0f, 20.0f, 20.0f));
			break;

		case EParkourArrowType::Down:
			Arrows[i]->ArrowColor = FColor::White;
			Arrows[i]->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
			break;
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
}

void AOnlyUpCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnlyUpGameMode = Cast<AOnlyUpGameMode>(GetWorld()->GetAuthGameMode());
}

void AOnlyUpCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AOnlyUpCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &ThisClass::Run);
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Completed, this, &ThisClass::Walk);

		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, MoveComponent, &UMoveComponent::DoCrouch);
	}
}

void AOnlyUpCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SpawnIndex);
}

void AOnlyUpCharacter::Action()
{
	Super::Action();

}

void AOnlyUpCharacter::SetModeAndCollision_NMC_Implementation(EMovementMode InMovementMode, bool InCollision)
{
	// 파쿠르 시작하면 충돌체 끄면서 모드 변경
	SetActorEnableCollision(InCollision);
	GetCharacterMovement()->SetMovementMode(InMovementMode);
}

void AOnlyUpCharacter::SetModeAndCollision_Server_Implementation(EMovementMode InMovementMode, bool InCollision)
{
	SetModeAndCollision_NMC(InMovementMode, InCollision);
}

void AOnlyUpCharacter::SetModeAndCollision(EMovementMode InMovementMode, bool InCollision)
{
	
	SetModeAndCollision_Server(InMovementMode, InCollision);
	
}

void AOnlyUpCharacter::Jump()
{
	if (StateComponent->IsIdleMode())
	{
		Super::Jump();
	}

	if (GetMovementComponent()->IsCrouching() == true)
	{
		MoveComponent->UnCrouch();
	}
}

void AOnlyUpCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (GetMovementComponent()->IsCrouching() == true)
	{
		MoveComponent->UnCrouch();
	}
}

void AOnlyUpCharacter::Walk_NMC_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
}

void AOnlyUpCharacter::Walk_Server_Implementation()
{
	Walk_NMC();
}

void AOnlyUpCharacter::Walk()
{
	Walk_Server();
}

void AOnlyUpCharacter::Run_NMC_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void AOnlyUpCharacter::Run_Server_Implementation()
{
	Run_NMC();
}

void AOnlyUpCharacter::Run()
{
	Run_Server();
}

void AOnlyUpCharacter::PlayerMaterialEventOnSpawn_Implementation()
{

}

void AOnlyUpCharacter::SetSpawnIndex_NMC_Implementation(int32 InIndex)
{
	if (InIndex >= SpawnIndex)
	{
		SpawnIndex = InIndex;
	}
}

void AOnlyUpCharacter::SetSpawnIndex_Server_Implementation(int32 InIndex)
{
	SetSpawnIndex_NMC(InIndex);
}

void AOnlyUpCharacter::SetSpawnIndex(int32 InIndex)
{
	SetSpawnIndex_Server(InIndex);
}

void AOnlyUpCharacter::RespawnPlayer(FTransform InTransform)
{
	if (OnlyUpGameMode)
		OnlyUpGameMode->RespawnPlayer(InTransform, GetController());

	Destroy();
}


