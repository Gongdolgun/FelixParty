#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "Weapon_Melee.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AWeapon_Melee : public AWeapon
{
	GENERATED_BODY()

public:
	AWeapon_Melee();

protected:
	void BeginPlay() override;

public:
	void Tick(float DeltaSeconds) override;

public:
	void Attack() override;

};
