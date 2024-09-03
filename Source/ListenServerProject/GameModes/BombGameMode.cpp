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

ABombGameMode::ABombGameMode()
{
	PlayerControllers = TArray<ADefaultController*>();

	bReplicates = true;

}

void ABombGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void ABombGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ADefaultController* Controller = Cast<ADefaultController>(NewPlayer);

	if (Controller)
	{
		PlayerControllers.Add(Controller);

		//GetWorldTimerManager().SetTimer(SpawnCharacterTimerHandle, this, &ABombGameMode::RandomSpawn, 3.0f, false);

		GetWorldTimerManager().SetTimer(GameStartTimerHandle, this, &ABombGameMode::StartGame, 3.0f, false);

		// 새로운 플레이어의 점수 초기화
		if (ABombGameState* gameState = Cast<ABombGameState>(GetWorld()->GetGameState()))
		{
			FPlayerInGameData newPlayerData;
			newPlayerData.PlayerName = Controller->GetName();
			newPlayerData.Score = 0;
			gameState->PlayerDatas.Add(newPlayerData);
		}
	}
}

void ABombGameMode::RandomSpawn()
{
	if (PlayerControllers.Num() == 0) 
		return;

	if (BombClass)
	{
		// 모든 플레이어 리스트 생성
		TArray<ABombCharacter*> allPlayers;
		for (ADefaultController* Controller : PlayerControllers)
		{
			ABombCharacter* character = Cast<ABombCharacter>(Controller->GetPawn());
			if (character && character->IsAlive())
			{
				allPlayers.Add(character);
			}
		}

		// 새로운 폭탄 소유자를 무작위로 선택
		if (allPlayers.Num() > 0)
		{
			int32 randomIndex = FMath::RandRange(0, allPlayers.Num() - 1);
			ABombCharacter* newBombOwner = allPlayers[randomIndex];

			// 새로운 소유자에게 폭탄 생성 명령
			if (newBombOwner)
			{
				newBombOwner->ServerSpawnBomb(BombClass);
			}

			// 모든 플레이어에서 기존 폭탄 제거
			for (ABombCharacter* character : allPlayers)
			{
				if (character && character != newBombOwner)
				{
					character->Bomb = nullptr;
					character->bBomb = false;
					character->GetCharacterMovement()->MaxWalkSpeed = character->GetCurrentMovementSpeed();
				}
			}
		}
	}
}

void ABombGameMode::OnPlayerDead(ABombCharacter* DeadPlayer)
{
	// 모든 플레이어 리스트 생성
	TArray<ABombCharacter*> allPlayers;
	for (ADefaultController* Controller : PlayerControllers)
	{
		ABombCharacter* Character = Cast<ABombCharacter>(Controller->GetPawn());
		if (Character && !Character->IsHidden())
		{
			allPlayers.Add(Character);
		}
	}

	// 게임 종료 조건 확인
	CheckGameEnd();

	ABombGameState* gameState = Cast<ABombGameState>(GetWorld()->GetGameState());
	if (gameState)
	{
		// 남아있는 플레이어에게 점수 부여
		for (ABombCharacter* character : allPlayers)
		{
			ADefaultController* characterController = Cast<ADefaultController>(character->GetController());
			if (character && character != DeadPlayer && character->IsAlive() && characterController)
			{
				FString characterName = characterController->GetPlayerState<APlayerState>()->GetPlayerName();
				gameState->UpdatePlayerScore(characterName, 5);
			}
		}
	}

	// 새로운 폭탄 소유자 선택
	if (allPlayers.Num() > 1)
	{
		int32 randomIndex = FMath::RandRange(0, allPlayers.Num() - 1);
		ABombCharacter* newBombOwner = allPlayers[randomIndex];
		if (newBombOwner)
		{
			newBombOwner->ServerSpawnBomb(BombClass);
		}
	}

}

void ABombGameMode::CheckGameEnd()
{
	// 남아있는 플레이어 수 확인
	int32 remainingPlayers = 0;
	ABombCharacter* lastPlayer = nullptr;
	ABombGameState* gameState = Cast<ABombGameState>(GetWorld()->GetGameState());

	for (ADefaultController* Controller : PlayerControllers)
	{
		ABombCharacter* character = Cast<ABombCharacter>(Controller->GetPawn());
		if (character && !character->IsHidden())
		{
			remainingPlayers++;
			lastPlayer = character;
		}
	}

	// 게임 종료 조건 확인
	if (remainingPlayers <= 1)
	{
		// 게임 종료 로직 (예: 승리자 발표, 게임 리셋 등)
		if (lastPlayer && gameState)
		{
			FString characterName = lastPlayer->GetController()->GetPlayerState<APlayerState>()->GetPlayerName();
			int32 playerScore = gameState->GetPlayerScore(characterName);

			CLog::Log(*lastPlayer->GetName());
			CLog::Log(playerScore);

		}

		// 게임을 재시작하거나 종료하는 로직을 추가
	}
}

void ABombGameMode::StartGame()
{
	// 게임 시작 시 모든 플레이어의 움직임 정지
	for (ADefaultController* Controller : PlayerControllers)
	{
		if (ABombCharacter* Character = Cast<ABombCharacter>(Controller->GetPawn()))
		{
			Character->GetCharacterMovement()->DisableMovement(); // 움직임 비활성화
		}
	}

	// 10초 후에 플레이어의 움직임을 재개하고 폭탄을 스폰
	GetWorldTimerManager().SetTimer(GameStartTimerHandle, this, &ABombGameMode::EnableMovementAndSpawnBomb, 1.0f, false);

	//GetWorldTimerManager().SetTimer(SpawnCharacterTimerHandle, this, &ABombGameMode::RandomSpawn, 10.0f, false);
}

void ABombGameMode::EnableMovementAndSpawnBomb()
{
	for (ADefaultController* Controller : PlayerControllers)
	{
		if (ABombCharacter* character = Cast<ABombCharacter>(Controller->GetPawn()))
		{
			character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}

	RandomSpawn();
}

void ABombGameMode::SetHolderController(ADefaultController* NewController)
{
	BombHolderController = NewController;
}
