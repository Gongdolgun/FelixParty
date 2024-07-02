#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "Weapon_Grenade.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AWeapon_Grenade : public AWeapon
{
	GENERATED_BODY()

public:
	AWeapon_Grenade();

protected:
	void BeginPlay() override;

public:
	void Tick(float DeltaSeconds) override;

public:
	virtual void Throw();

};
