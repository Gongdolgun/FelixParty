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
	ChangeType(StartWeapon);
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UWeaponComponent::Begin_Fire()
{
	if(CurWeapon != nullptr)
		CurWeapon->BeginFire();
}

void UWeaponComponent::End_Fire()
{

}

void UWeaponComponent::ChangeType(TSubclassOf<class AWeapon> NewWeapon)
{
	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AWeapon* SpawnedWeapon = Owner->GetWorld()->SpawnActor<AWeapon>(NewWeapon, params);

	if(SpawnedWeapon != nullptr)
	{
		EWeaponType type = Type;
		Type = SpawnedWeapon->WeaponType;

		if (OnWeaponTypeChange.IsBound())
			OnWeaponTypeChange.Broadcast(type, Type);

		SetCurrentWeapon(SpawnedWeapon);
	}
}

void UWeaponComponent::SetCurrentWeapon(class AWeapon* NewWeapon)
{
	if(CurWeapon)
		CurWeapon->Destroy();

	CurWeapon = NewWeapon;
}
