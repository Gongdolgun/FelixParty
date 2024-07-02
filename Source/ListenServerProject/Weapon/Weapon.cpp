#include "Weapon/Weapon.h"
#include "Global.h"
#include "Components/WeaponComponent.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateActorComponent<UWeaponComponent>(this, &WeaponComponent, "WeaponComponent");
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Equip()
{
	if (WeaponComponent)
	{
		WeaponComponent->Equip();
	}
}

void AWeapon::AllEquip()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeapon::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AWeapon* Child = Cast<AWeapon>(Actor);
		if (Child)
		{
			Child->Equip();
		}
	}
}

