#include "GameModes/BombGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Engine/TargetPoint.h"
#include "Global.h"
#include "Characters/BombCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "SpawnActor/Bomb.h"
#include "Utilites/CLog.h"
#include "Controllers/BombController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameState/BombGameState.h"
#include "GameState/DefaultGameState.h"
#include "Widgets/BombGameStartMessage.h"

void ABombGameMode::BeginPlay()
{
	Super::BeginPlay();

	ABombGameState* BombGameState = GetGameState<ABombGameState>();
	if (BombGameState)
	{
		BombGameState->OnGameStateTypeChanged.AddDynamic(this, &ABombGameMode::CreateBombSpawn);
	}
}

void ABombGameMode::CreateBombSpawn(EGameStateType InPrevGameType, EGameStateType InNewGameType)
{
	if (InNewGameType == EGameStateType::GameStart)
	{
		SurvivedControllers = ConnectedPlayers;

		//FTimerHandle SpawnTimerHandle;

		//GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ABombGameMode::SpawnBomb, 3.0f, false);

		SpawnBomb();
	}
}

void ABombGameMode::OnPlayerDead(ABombCharacter* DeadPlayer)
{

}

void ABombGameMode::SpawnBomb()
{
	if (SurvivedControllers.IsEmpty())
		return;

	if (BombClass)
	{
		int Randominteger = UKismetMathLibrary::RandomIntegerInRange(0, SurvivedControllers.Num() - 1);

		ABombCharacter* SelectedCharacter = Cast<ABombCharacter>(SurvivedControllers[Randominteger]->GetPawn());
		if (SelectedCharacter != nullptr)
		{
			SpawnedBomb = Cast<ABomb>(GetWorld()->SpawnActor(BombClass));

			HitAttachBomb(SelectedCharacter);
		}
	}
}

void ABombGameMode::HitAttachBomb(ABombCharacter* InCharacter)
{
	if (SpawnedBomb != nullptr)
	{
		SpawnedBomb->AttachBomb(InCharacter);
	}
}

void ABombGameMode::SomeoneDead(ADefaultController* InController)
{
	SurvivedControllers.Remove(InController);

	for (auto SurvivedController : SurvivedControllers)
	{
		ABombGameState* BombGameState = GetGameState<ABombGameState>();
		if (BombGameState)
		{
			FString PlayerName = SurvivedController->GetPlayerState<APlayerState>()->GetPlayerName();
			BombGameState->UpdatePlayerScore(PlayerName, 5);
		}
	}

	if (SurvivedControllers.Num() == 1)
	{
		ADefaultGameState* DefaultGameState = GetGameState<ADefaultGameState>();
		if (DefaultGameState != nullptr)
		{
			DefaultGameState->SetGameState(EGameStateType::GameOver);
			return;
		}
	}

	SpawnBomb();
}