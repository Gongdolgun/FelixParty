#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "Pistol.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API APistol : public AWeapon
{
	GENERATED_BODY()

public:
	APistol();

	virtual void Equip();

};
