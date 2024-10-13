#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

UENUM(BlueprintType)
enum class EGameStateType : uint8
{
	Ready = 0, GameStart, GamePlay, GameOver, InGameRankBoard, TotalRankBoard, Max
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

UENUM(BlueprintType)
enum class EHudTypes : uint8
{
	None = 0, Ready, Countdown, GamePlay, GameOver, InGameRankBoard, TotalRankBoard
};

UENUM(BlueprintType)
enum class EOptionTypes : uint8
{
	None = 0, GamePlayOption, EmoteOption
};

UENUM(BlueprintType)
enum class EActorRotationType : uint8
{
	None = 0, Pitch, Yaw, Roll
};


UENUM(BlueprintType)
enum class ELevelInputType : uint8
{
	None = 0, Up, Down, Right, Left, Shift, 
};