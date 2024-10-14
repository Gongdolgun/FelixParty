#include "Actors/Spawner_TotalCharacter.h"
#include "TotalScoreCharacter.h"
#include "GameState/DefaultGameState.h"

void ASpawner_TotalCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASpawner_TotalCharacter::SpawnActor()
{
	ADefaultGameState* DefaultGameState = Cast<ADefaultGameState>(GetWorld()->GetGameState());

	if (HasAuthority())
	{
		if (DefaultGameState != nullptr && !SpawnActorClass.IsEmpty())
		{
			for (int32 i = 0; i < DefaultGameState->PlayerDatas.Num(); i++)
			{
				FVector location = GetActorLocation() + FVector(0, 0, 100);

				FActorSpawnParameters params;
				params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				ATotalScoreCharacter* SpawnedCharacter = GetWorld()->SpawnActor<ATotalScoreCharacter>(SpawnActorClass[0], location, FRotator::ZeroRotator, params);

				if (SpawnedCharacter != nullptr)
				{
					SpawnedCharacter->Number = i;
					SpawnedCharacter->Init();
				}
			}
		}
	}
}
