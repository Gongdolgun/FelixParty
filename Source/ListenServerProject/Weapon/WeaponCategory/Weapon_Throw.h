#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "Weapon_Throw.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AWeapon_Throw : public AWeapon
{
	GENERATED_BODY()

public:
	AWeapon_Throw();

protected:
	void BeginPlay() override;

public:
	void Tick(float DeltaSeconds) override;

public:
	void Equip() override;

};
