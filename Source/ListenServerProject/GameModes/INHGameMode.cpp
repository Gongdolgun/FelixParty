#include "GameModes/INHGameMode.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "Characters/INHCharacter.h"

void AINHGameMode::SetGhostMode(AController* InController)
{
    if (InController != nullptr)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            AINHCharacter* Character = Cast<AINHCharacter>(InController->GetPawn());
            if (Character)
            {
                FTransform SpawnTransform = Character->Camera->GetComponentTransform();
                Character->DeadMaterialEvent();

                FActorSpawnParameters params;
                params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                APawn* NewGhost = World->SpawnActor<APawn>(Ghost, SpawnTransform, params);

                if (NewGhost)
                    InController->Possess(NewGhost);
            }
        }
    }
}
