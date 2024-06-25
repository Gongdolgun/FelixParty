// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPS_ListenServerGameMode.h"
#include "TPS_ListenServerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATPS_ListenServerGameMode::ATPS_ListenServerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
