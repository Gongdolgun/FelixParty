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
                // 이전 캐릭터의 SavePoint 값을 가져와서 설정
                AOnlyUpCharacter* PreviousCharacter = Cast<AOnlyUpCharacter>(InController->GetPawn());
                if (PreviousCharacter)
                {
                    RespawnCharacter->SetSpawnIndex(PreviousCharacter->SpawnIndex);
                }

                InController->Possess(RespawnCharacter);
                UpdatePlayer();
                RespawnCharacter->PlayMaterialEvent();
            }
        }
    }
}


