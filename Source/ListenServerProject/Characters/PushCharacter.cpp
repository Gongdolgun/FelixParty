#include "Characters/PushCharacter.h"
#include "Global.h"
#include "Actors/Push/PushRespawner.h"
#include "Components/MoveComponent.h"
#include "Components/StateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

APushCharacter::APushCharacter()
{
	Helpers::CreateActorComponent<UStateComponent>(this, &StateComponent, "State");

	HP = 100.0f;

}

void APushCharacter::BeginPlay()
{
	Super::BeginPlay();

	MoveComponent->EnableControlRotation();
	StateComponent->SetIdleMode();

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

	
}

void APushCharacter::Action()
{
	Super::Action();

	if (StateComponent->IsIdleMode() == false) return;
	if (GetCharacterMovement()->IsFalling() == true) return;

	StateComponent->SetActionMode();

	if (HasAuthority())
	{
		PlayActionMontage_NMC(EStateType::Action);
	}

	else
	{
		PlayActionMontage_Server(EStateType::Action);
	}
}

void APushCharacter::Hit(AActor* InActor, const FHitData& InHitData)
{
	Super::Hit(InActor, InHitData);

	HP = UKismetMathLibrary::Clamp(HP - InHitData.Damage, 0, MaxHP);

	if (HP > 0.0f)
	{
		StateComponent->SetHittedMode();

		if (HasAuthority())
		{
			PlayActionMontage_NMC(EStateType::Hitted);
		}

		else
		{
			PlayActionMontage_Server(EStateType::Hitted);
		}

		FVector launch = InActor->GetActorForwardVector() * 1500.0f;
		LaunchCharacter(launch, true, false);
	}

	else
	{
		FVector ImpulseDirection = InActor->GetActorForwardVector() * 10000.f;
		Dead_NMC(ImpulseDirection);

		// 랜덤 위치 캐릭터 스폰
		//RespawnCharacter_Server(this);
		FTimerHandle SpawnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &APushCharacter::RespawnCharacter_Server, 2.0f, false);
	}
	
}

void APushCharacter::Dead_NMC_Implementation(FVector InImpulseDirection)
{
	MoveComponent->CanMove = false;
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	InImpulseDirection.Z = 3000.f;

	GetMesh()->AddImpulse(InImpulseDirection, NAME_None, true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void APushCharacter::SpawnObject_Server_Implementation(UClass* InClass, FTransform InTransform)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	GetWorld()->SpawnActor(InClass, &InTransform, SpawnParams);
}


void APushCharacter::PlayActionMontage_NMC_Implementation(EStateType InStateType)
{
	if (InStateType == EStateType::Action)
	{
		if (AttackMontage == nullptr)
		{
			CLog::Print("AttackMontage Nullptr");

			return;
		}
		
		PlayAnimMontage(AttackMontage);
	}

	else if (InStateType == EStateType::Hitted)
	{
		if (HittedMontage == nullptr)
		{
			CLog::Print("HittedMontage Nullptr");

			return;
		}
	
		PlayAnimMontage(HittedMontage);
	}
	
}

void APushCharacter::PlayActionMontage_Server_Implementation(EStateType InStateType)
{
	PlayActionMontage_NMC(InStateType);
}

void APushCharacter::RespawnCharacter_Server_Implementation()
{
	APushRespawner* spawner = Cast<APushRespawner>(UGameplayStatics::GetActorOfClass(GetWorld(), Respawner));
	if (spawner == nullptr) return;

	FVector spawnLocation = spawner->GetSpawnCollisionRandomPoint();
	FRotator spawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APushCharacter* respawnCharacter = GetWorld()->SpawnActor<APushCharacter>(RespawnCharacter, spawnLocation, spawnRotation, params);
	if (respawnCharacter)
	{
		AController* controller = this->GetController();
		if (controller)
		{
			controller->Possess(respawnCharacter);
		}
	}

}
