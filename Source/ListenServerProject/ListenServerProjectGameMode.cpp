// Copyright Epic Games, Inc. All Rights Reserved.

#include "ListenServerProjectGameMode.h"
#include "ListenServerProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AListenServerProjectGameMode::AListenServerProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/Player/BP_UserPlayer.BP_UserPlayer_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
