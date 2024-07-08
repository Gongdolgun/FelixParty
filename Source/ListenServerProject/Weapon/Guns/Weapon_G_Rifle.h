#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponCategory/Weapon_Guns.h"
#include "Weapon_G_Rifle.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AWeapon_G_Rifle : public AWeapon_Guns
{
	GENERATED_BODY()

public:
	AWeapon_G_Rifle();

public:
	void Begin_Equip() override;
	void End_Equip() override;

};
