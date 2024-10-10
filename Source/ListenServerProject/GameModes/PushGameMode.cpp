#include "GameModes/PushGameMode.h"
#include "Actors/Push/ElectricField.h"
#include "Characters/PushCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Global.h"
#include "GameState/PushGameState.h"

APushGameMode::APushGameMode()
{
    PrimaryActorTick.bCanEverTick = true;


}

void APushGameMode::BeginPlay()
{
    Super::BeginPlay();

    PushGameState = GetGameState<APushGameState>();

    SpawnElectricField();
}

void APushGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);


}

void APushGameMode::SpawnElectricField()
{
    FActorSpawnParameters params;
    params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FTransform transform;
    transform.SetLocation(FVector::ZeroVector);
    transform.SetScale3D(FVector(80.0f, 80.0f, 80.0f));

    if (ElectricFieldClass)
        ElectricField = GetWorld()->SpawnActor<AElectricField>(ElectricFieldClass, transform, params);
}

void APushGameMode::RespawnPlayer(AController* InController)
{
    if (InController != nullptr && PushGameState->GetGameStateType() == EGameStateType::GamePlay)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            RespawnRadius = ElectricField->CurrFieldRadius - 50.0f;

            // 랜덤 위치 세팅
            FVector RandomLocation = FVector(
                FMath::RandRange(-RespawnRadius, RespawnRadius),
                FMath::RandRange(-RespawnRadius, RespawnRadius),
                65.0f // Z 위치
            );

            FActorSpawnParameters params;
            params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            APushCharacter* RespawnCharacter = World->SpawnActor<APushCharacter>(DefaultPawnClass, FTransform(RandomLocation), params);

            if (RespawnCharacter)
            {
                InController->Possess(RespawnCharacter);
                UpdatePlayer();
                RespawnCharacter->PlayMaterialEvent();
            }
            
        }
    }
}

