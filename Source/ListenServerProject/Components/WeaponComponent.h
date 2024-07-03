#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType
{
	Pistol, Rifle, Shotgun, Sniper, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, PrevType, EWeaponType, NewType);

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

private:
	class AWeapon* GetCurrWeapon();

public:
	void Begin_Equip();
	void End_Equip();

	void Begin_Fire();
	void End_Fire();

public:
	void SetPistolMode();
	void SetRifleMode();
	void SetShotgunMode();
	void SetSniperMode();

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

private:
	EWeaponType Type = EWeaponType::Max;

public:
	FWeaponTypeChanged OnWeaponTypeChange;

private:
	class AListenServerProjectCharacter* Owner;

	AWeapon* Weapon;

	TArray<class AWeapon*> Weapons;


};
