#include "Components/WeaponComponent.h"
#include "Global.h"
#include "Weapon/Weapon.h"
#include "Characters/DefaultCharacter.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());

	// 초기 세팅은 총을 들고 시작
	Type = EWeaponType::Gun;

	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (StartWeapon)
	{
		AWeapon* weapon = Owner->GetWorld()->SpawnActor<AWeapon>(StartWeapon, params);
		SetCurrentWeapon(weapon);
	}
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UWeaponComponent::Begin_Fire()
{
	if(CurWeapon)
		CurWeapon->Fire();
}

void UWeaponComponent::End_Fire()
{

}

void UWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType type = Type;
	Type = InType;

	if (OnWeaponTypeChange.IsBound())
		OnWeaponTypeChange.Broadcast(type, InType);
}

void UWeaponComponent::SetCurrentWeapon(class AWeapon* NewWeapon)
{
	CurWeapon = NewWeapon;
}
