#include "Characters/PushCharacter.h"
#include "Global.h"
#include "Actors/Push/PushRespawner.h"
#include "Components/MoveComponent.h"
#include "Components/StateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameModes/PushGameMode.h"
#include "Net/UnrealNetwork.h"

APushCharacter::APushCharacter()
{
	Helpers::CreateActorComponent<UStateComponent>(this, &StateComponent, "State");

	HP = 100.0f;

}

void APushCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (MoveComponent)
		MoveComponent->EnableControlRotation();

	if (StateComponent)
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
		PlayActionMontage_NMC(AttackMontage);
	}

	else
	{
		PlayActionMontage_Server(AttackMontage);
	}
}

void APushCharacter::Jump()
{
	if (StateComponent->IsIdleMode())
	{
		Super::Jump();

	}
}

void APushCharacter::Hit(AActor* InActor, const FHitData& InHitData)
{
	Super::Hit(InActor, InHitData);

	HP = UKismetMathLibrary::Clamp(HP - InHitData.Damage, 0, MaxHP);

	if (HP > 0.0f)
	{
		// 몽타주가 있을 때만
		if (InHitData.Montage)
		{
			StateComponent->SetHittedMode();

			if (HasAuthority())
				PlayActionMontage_NMC(InHitData.Montage);

			else
				PlayActionMontage_Server(InHitData.Montage);
		}
		
		LaunchCharacter(InHitData.Launch, true, false);
	}

	else
	{
		//FVector ImpulseDirection = InActor->GetActorForwardVector() * 1000.f;
		Dead_NMC();

		// 랜덤 위치 캐릭터 스폰
		//if (HasAuthority())
		//{
		//	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &APushCharacter::OnRespawnCharacter_Server, 2.0f, false);
		//}

		RespawnCharacter();
	}
	
}

void APushCharacter::RespawnCharacter()
{
	APushGameMode* PushGameMode = Cast<APushGameMode>(GetWorld()->GetAuthGameMode());
	if (PushGameMode)
	{
		PushGameMode->RespawnPlayer(GetController());

		CLog::Print("RespawnPlayer 1");
	}

	Destroy();
}


void APushCharacter::Dead_NMC_Implementation()
{
	MoveComponent->CanMove = false;
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	//InImpulseDirection.Z = 500.f;
	//GetMesh()->AddImpulse(InImpulseDirection, NAME_None, true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void APushCharacter::SpawnObject_Server_Implementation(UClass* InClass, FTransform InTransform)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	GetWorld()->SpawnActor(InClass, &InTransform, SpawnParams);
}


void APushCharacter::PlayActionMontage_NMC_Implementation(UAnimMontage* InMontage)
{
	if (InMontage)
		PlayAnimMontage(InMontage);
}

void APushCharacter::PlayActionMontage_Server_Implementation(UAnimMontage* InMontage)
{
	if (InMontage)
		PlayActionMontage_NMC(InMontage);
}

//void APushCharacter::OnRespawnCharacter_Server_Implementation()
//{
//	APushRespawner* spawner = Cast<APushRespawner>(UGameplayStatics::GetActorOfClass(GetWorld(), Respawner));
//	if (spawner == nullptr) return;
//
//	FVector spawnLocation = spawner->GetSpawnCollisionRandomPoint();
//	FRotator spawnRotation = FRotator::ZeroRotator;
//	FActorSpawnParameters params;
//	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	APushCharacter* respawnCharacter = GetWorld()->SpawnActor<APushCharacter>(RespawnCharacter, spawnLocation, spawnRotation, params);
//	if (respawnCharacter)
//	{
//		AController* controller = this->GetController();
//		if (controller)
//		{
//			controller->Possess(respawnCharacter);
//
//			GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
//		}
//	}
//}