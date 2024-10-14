#include "GameModes/OnlyUpGameMode.h"
#include "Global.h"
#include "Characters/OnlyUpCharacter.h"
#include "GameState/OnlyUpGameState.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/DefaultHUD.h"

AOnlyUpGameMode::AOnlyUpGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOnlyUpGameMode::BeginPlay()
{
	Super::BeginPlay();

    OnlyUpGameState = GetGameState<AOnlyUpGameState>();
}

void AOnlyUpGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AOnlyUpGameMode::RespawnPlayer(FTransform InTransform, AController* InController)
{
    if (InController != nullptr && OnlyUpGameState->GetGameStateType() == EGameStateType::GamePlay)
	   {
        UWorld* World = GetWorld();
        if (World)
        {
            // Spawn 위치 세팅
            FActorSpawnParameters params;
            params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            AOnlyUpCharacter* RespawnCharacter = World->SpawnActor<AOnlyUpCharacter>(DefaultPawnClass, InTransform, params);

            if (RespawnCharacter)
            {
                InController->Possess(RespawnCharacter);
                UpdatePlayer();
                RespawnCharacter->PlayMaterialEvent();
            }

        }
    }
}

