#include "GameModes/FPSGameMode.h"
#include "Global.h"
#include "Characters/FPSCharacter.h"
#include "GameFramework/PlayerStart.h"

void AFPSGameMode::RespawnPlayer(AController* controller)
{
    if (controller != nullptr)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            TArray<AActor*> PlayerStarts;
            UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), PlayerStarts);

            if (PlayerStarts.Num() > 0)
            {
                int32 RandomIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);

                FActorSpawnParameters params;
                params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                AFPSCharacter* NewCharacter = World->SpawnActor<AFPSCharacter>(DefaultPawnClass, PlayerStarts[RandomIndex]->GetActorTransform(), params);

                if (NewCharacter)
                {
                    controller->Possess(NewCharacter);
                    UpdatePlayer();
                    NewCharacter->PlayMaterialEvent();
                }
            }
        }
    }
}
