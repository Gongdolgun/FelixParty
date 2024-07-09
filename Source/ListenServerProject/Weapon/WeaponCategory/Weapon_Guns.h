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
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;
};
