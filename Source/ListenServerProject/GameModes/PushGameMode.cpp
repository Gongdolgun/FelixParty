#include "GameModes/PushGameMode.h"
#include "Actors/Push/ElectricField.h"
#include "Characters/PushCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Global.h"

APushGameMode::APushGameMode()
{

}

void APushGameMode::BeginPlay()
{
	Super::BeginPlay();

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
	transform.SetScale3D(FVector(50.0f, 50.0f, 50.0f));

	if (ElectricFieldClass)
	{
		ElectricField = GetWorld()->SpawnActor<AElectricField>(ElectricFieldClass, transform, params);

	}
}

void APushGameMode::RespawnPlayer(AController* InController)
{
    if (InController != nullptr)
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
                APushCharacter* NewCharacter = World->SpawnActor<APushCharacter>(DefaultPawnClass, PlayerStarts[RandomIndex]->GetActorTransform(), params);


                if (NewCharacter)
                {
                    InController->Possess(NewCharacter);
                    UpdatePlayer();
                    NewCharacter->PlayMaterialEvent();
                }
            }
        }
    }
}
 


