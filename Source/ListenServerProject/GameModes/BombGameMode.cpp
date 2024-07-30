#include "GameModes/BombGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Engine/TargetPoint.h"
#include "Global.h"
#include "Characters/BombCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "SpawnActor/Bomb.h"
#include "Utilites/CLog.h"
#include "Controllers/BombController.h"

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

		GetWorldTimerManager().SetTimer(SpawnCharacterTimerHandle, this, &ABombGameMode::RandomSpawn, 3.0f, false);
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
		if (lastPlayer)
		{
			CLog::Log(*lastPlayer->GetName());
		}

		// 게임을 재시작하거나 종료하는 로직을 추가
	}
}

void ABombGameMode::SetHolderController(ADefaultController* NewController)
{
	BombHolderController = NewController;
}
