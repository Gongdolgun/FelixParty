#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponCategory/Weapon_Guns.h"
#include "Weapon_G_Pistol.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AWeapon_G_Pistol : public AWeapon_Guns
{
	GENERATED_BODY()

public:
	AWeapon_G_Pistol();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;
};
