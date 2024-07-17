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

            UE_LOG(LogTemp, Warning, TEXT("Number of PlayerStarts: %d"), PlayerStarts.Num()); // 로그 추가

            if (PlayerStarts.Num() > 0)
            {
                int32 RandomIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);

                FActorSpawnParameters params;
                params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                AFPSCharacter* NewCharacter = World->SpawnActor<AFPSCharacter>(DefaultPawnClass, params);

                if (NewCharacter)
                {
                    controller->Possess(NewCharacter);
                    UE_LOG(LogTemp, Warning, TEXT("Player respawned successfully."));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to spawn new character."));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("No PlayerStart found."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("World is not valid."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Controller is not valid."));
    }
}
