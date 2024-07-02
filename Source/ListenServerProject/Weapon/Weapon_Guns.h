#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "Weapon_Guns.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AWeapon_Guns : public AWeapon
{
	GENERATED_BODY()

public:
	AWeapon_Guns();

protected:
	void BeginPlay() override;

public:
	void Tick(float DeltaSeconds) override;

public:
	virtual void Fire_Implementation() override;

};
