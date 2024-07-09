#include "Characters/FPSCharacter.h"
#include "AnimInstance_DefaultCharacter.h"
#include "Global.h"
#include "Net/UnrealNetwork.h"

AFPSCharacter::AFPSCharacter()
{
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	HP = MaxHP;
}

void AFPSCharacter::Hit(AActor* InActor, const FHitData& InHitData)
{
	Super::Hit(InActor, InHitData);

	HP = UKismetMathLibrary::Clamp(HP - InHitData.Damage, 0, MaxHP);
}

void AFPSCharacter::Action()
{
	WeaponComponent->Begin_Fire();
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, HP);
}
