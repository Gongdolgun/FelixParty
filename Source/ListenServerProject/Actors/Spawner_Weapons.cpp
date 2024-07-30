#include "Actors/Spawner_Weapons.h"
#include "Global.h"
#include "Characters/DefaultCharacter.h"
#include "Components/WeaponComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Weapon/Weapon.h"

ASpawner_Weapons::ASpawner_Weapons()
{
}

void ASpawner_Weapons::BeginPlay()
{
	Super::BeginPlay();

}

void ASpawner_Weapons::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADefaultCharacter* OverlappedCharacter = Cast<ADefaultCharacter>(OtherActor);
	if(OverlappedCharacter != nullptr)
	{
		if (IsValid(OverlappedCharacter->WeaponComponent))
		{
			if (SpawnedActor != nullptr && SpawnedActor->IsA(AWeapon::StaticClass()))
			{
				OverlappedCharacter->WeaponComponent->ChangeType(SpawnedActor->GetClass());

				SpawnedActor->Destroy();
				SpawnedActor = nullptr;

				GetWorld()->GetTimerManager().SetTimer(timer, this, &ThisClass::SpawnActor, SpawnTimeRate, false);
			}
		}
	}
}

void ASpawner_Weapons::SpawnActor()
{
	if (SpawnActorClass.Num() > 0)
	{
		int32 RandomInteger = UKismetMathLibrary::RandomIntegerInRange(0, SpawnActorClass.Num() - 1);

		if (SpawnActorClass[RandomInteger] != nullptr)
		{
			FVector Location = GetActorLocation() + SpawnLocation;

			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnActorClass[RandomInteger], Location, FRotator::ZeroRotator, params);

			CreateRotatingMovementComponent();
		}
	}
}