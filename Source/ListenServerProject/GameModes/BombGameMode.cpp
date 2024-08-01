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

		// 모든 플레이어가 로그인한 후 10초 동안 대기
        if (PlayerControllers.Num() == 1) // 첫 번째 플레이어가 로그인한 경우
        {
            GetWorldTimerManager().SetTimer(GameStartTimerHandle, this, &ABombGameMode::StartGame, 10.0f, false);
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

void ABombGameMode::StartGame()
{
	ShowMessage(TEXT("게임이 시작됩니다. 10초 후 폭탄이 랜덤으로 소유자에게 배정됩니다."));

	// 게임 시작 로직
	GetWorldTimerManager().SetTimer(SpawnCharacterTimerHandle, this, &ABombGameMode::RandomSpawn, 3.0f, false);
}

void ABombGameMode::ShowMessage(const FString& Message)
{
	for (AController* Controller : PlayerControllers)
	{
		if (Controller)
		{
			ADefaultController* PlayerController = Cast<ADefaultController>(Controller);
			if (PlayerController)
			{
				// UI 위젯 생성 및 메시지 표시
				UUserWidget* MessageWidget = CreateWidget<UUserWidget>(PlayerController->GetWorld(), MessageWidgetClass);
				if (MessageWidget)
				{
					MessageWidget->AddToViewport();
					// 위젯에 메시지를 전달하는 방법에 따라 추가 로직
				}
			}
		}
	}
}

void ABombGameMode::SetHolderController(ADefaultController* NewController)
{
	BombHolderController = NewController;
}
