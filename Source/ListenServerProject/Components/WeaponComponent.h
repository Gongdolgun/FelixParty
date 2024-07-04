#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

UENUM(BlueprintType)
enum class WeaponType
{
	Gun, Melee, Throw, Max,
};

UENUM(BlueprintType)
enum class GunType
{
	Pistol, Rifle, Shotgun, Sniper, Max,
};

UENUM(BlueprintType)
enum class MeleeType
{
	Fist, Knife, Club, Max,
};

UENUM(BlueprintType)
enum class Throw
{
	Grenade, Flashbang, Smokeshell, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, WeaponType, PrevType, WeaponType, NewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LISTENSERVERPROJECT_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	TArray<TSubclassOf<class AWeapon>> WeaponClass;

public:	
	UWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Begin_Equip(int num);
	void End_Equip();

	void EquipWeapon_1();

	void Begin_Fire();
	void End_Fire();

private:
	void SetMode(WeaponType InType);

	void SetCurrWeapon(AWeapon* NewWeapon);

	void ChangeType(WeaponType InType);

private:
	WeaponType Type = WeaponType::Max;

public:
	FWeaponTypeChanged OnWeaponTypeChange;

private:
	class ACharacter* Owner;

	AWeapon* Weapon;

	TArray<class AWeapon*> Weapons;

};
