#include "Characters/PushCharacter.h"
#include "Global.h"
#include "Components/MoveComponent.h"
#include "Components/StateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

APushCharacter::APushCharacter()
{
	Helpers::CreateActorComponent<UStateComponent>(this, &StateComponent, "State");

	//GetMesh()->SetSimulatePhysics(true);
}

void APushCharacter::BeginPlay()
{
	Super::BeginPlay();

	MoveComponent->EnableControlRotation();
}

void APushCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APushCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APushCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, HP);
}

void APushCharacter::Action()
{
	Super::Action();

	if (StateComponent->IsIdleMode() == false || GetCharacterMovement()->IsFalling() == true) return;

	StateComponent->SetActionMode();

	if (HasAuthority())
	{
		PlayAttackMontage_NMC();
	}

	else
	{
		PlayAttackMontage_Server();
	}
}

void APushCharacter::Hit(AActor* InActor, const FHitData& InHitData)
{
	Super::Hit(InActor, InHitData);

	//FMath::Clamp(HP, 0, 100.0f);

	HP -= InHitData.Damage;

	FVector launch = InActor->GetActorForwardVector() * 1500.0f;
	LaunchCharacter(launch, true, false);

	CLog::Print(HP);
}

void APushCharacter::Dead()
{

}

void APushCharacter::SpawnObject_Server_Implementation(UClass* InClass, FTransform InTransform)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	GetWorld()->SpawnActor(InClass, &InTransform, SpawnParams);
}


void APushCharacter::PlayAttackMontage_NMC_Implementation()
{
	if (AttackMontage == nullptr) return;

	PlayAnimMontage(AttackMontage);
}

void APushCharacter::PlayAttackMontage_Server_Implementation()
{
	PlayAttackMontage_NMC();
}
