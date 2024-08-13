#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

UENUM(BlueprintType)
enum class EGameStateType : uint8
{
	Ready = 0, GameStart, GamePlay, GameOver, Max
};

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle = 0, Action, Hitted, Dead, Max
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Gun = 0, Rifle, Shotgun, Max
};

UENUM(BlueprintType)
enum class EGunType : uint8
{
	Pistol = 0, Rifle, Shotgun, Sniper, Max,
};

UENUM(BlueprintType)
enum class EMeleeType : uint8
{
	Fist = 0, Knife, Club, Max,
};

UENUM(BlueprintType)
enum class EThrow : uint8
{
	Grenade = 0, Flashbang, Smokeshell, Max,
};

UENUM(BlueprintType)
enum class EParkourArrowType : uint8
{
	Left = 0, Center, Right, Down, Max
};

UENUM(BlueprintType)
enum class EParkourType : uint8
{
	High = 0, Low, Jump, Max
};

UENUM(BlueprintType)
enum class EPlayerColors : uint8
{
	Red = 0, Black, White
};