#include "Components/WeaponComponent.h"
#include "Global.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UWeaponComponent::Equip()
{
	printf("부모 Equip 호출");
	UE_LOG(LogTemp, Warning, TEXT("부모 Equip 호출"));

}

void UWeaponComponent::SetPistolMode()
{
	SetMode(EWeaponType::Pistol);
}

void UWeaponComponent::SetARMode()
{
	SetMode(EWeaponType::AR);
}

void UWeaponComponent::SetShotgunMode()
{
	SetMode(EWeaponType::Shotgun);
}

void UWeaponComponent::SetSniperMode()
{
	SetMode(EWeaponType::Sniper);
}

void UWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
		return;

	else
	{
		ChangeType(InType);
	}
}

void UWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType type = Type;
	Type = InType;

	if (OnWeaponTypeChange.IsBound())
		OnWeaponTypeChange.Broadcast(type, InType);
}

