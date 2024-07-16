#include "GameModes/FPSGameMode.h"
#include "Global.h"
#include "Characters/FPSCharacter.h"
#include "GameFramework/PlayerStart.h"

void AFPSGameMode::RespawnPlayer(AController* controller)
{
	if(controller != nullptr)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

		if (PlayerStarts.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);

			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AFPSCharacter* NewCharacter = GetWorld()->SpawnActor<AFPSCharacter>(DefaultPawnClass, params);

			controller->Possess(NewCharacter);
		}
	}
}
