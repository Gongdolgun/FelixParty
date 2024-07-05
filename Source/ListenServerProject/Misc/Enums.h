#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle = 0, Action, Hitted, Dead, Max
};

UENUM(BlueprintType)
enum class EWeaponType
{
	Gun = 0, Melee, Throw, Max
};

UENUM(BlueprintType)
enum class EGunType
{
	Pistol = 0, Rifle, Shotgun, Sniper, Max,
};

UENUM(BlueprintType)
enum class EMeleeType
{
	Fist = 0, Knife, Club, Max,
};

UENUM(BlueprintType)
enum class EThrow
{
	Grenade = 0, Flashbang, Smokeshell, Max,
};
