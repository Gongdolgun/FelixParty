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

		// ��� �÷��̾ �α����� �� 10�� ���� ���
        if (PlayerControllers.Num() == 1) // ù ��° �÷��̾ �α����� ���
        {
            GetWorldTimerManager().SetTimer(GameStartTimerHandle, this, &ABombGameMode::StartGame, 3.0f, false);
        }

		// ���ο� �÷��̾��� ���� �ʱ�ȭ
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
		// ��� �÷��̾� ����Ʈ ����
		TArray<ABombCharacter*> allPlayers;
		for (ADefaultController* Controller : PlayerControllers)
		{
			ABombCharacter* character = Cast<ABombCharacter>(Controller->GetPawn());
			if (character && character->IsAlive())
			{
				allPlayers.Add(character);
			}
		}

		// ���ο� ��ź �����ڸ� �������� ����
		if (allPlayers.Num() > 0)
		{
			int32 randomIndex = FMath::RandRange(0, allPlayers.Num() - 1);
			ABombCharacter* newBombOwner = allPlayers[randomIndex];

			// ���ο� �����ڿ��� ��ź ���� ���
			if (newBombOwner)
			{
				newBombOwner->ServerSpawnBomb(BombClass);
			}

			// ��� �÷��̾�� ���� ��ź ����
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
	// ��� �÷��̾� ����Ʈ ����
	TArray<ABombCharacter*> allPlayers;
	for (ADefaultController* Controller : PlayerControllers)
	{
		ABombCharacter* Character = Cast<ABombCharacter>(Controller->GetPawn());
		if (Character && !Character->IsHidden())
		{
			allPlayers.Add(Character);
		}
	}

	// ���� ���� ���� Ȯ��
	CheckGameEnd();

	ABombGameState* gameState = Cast<ABombGameState>(GetWorld()->GetGameState());
	if (gameState)
	{
		// �����ִ� �÷��̾�� ���� �ο�
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

	// ���ο� ��ź ������ ����
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
	// �����ִ� �÷��̾� �� Ȯ��
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

	// ���� ���� ���� Ȯ��
	if (remainingPlayers <= 1)
	{
		// ���� ���� ���� (��: �¸��� ��ǥ, ���� ���� ��)
		if (lastPlayer && gameState)
		{
			FString characterName = lastPlayer->GetController()->GetPlayerState<APlayerState>()->GetPlayerName();
			int32 playerScore = gameState->GetPlayerScore(characterName);

			CLog::Log(*lastPlayer->GetName());
			CLog::Log(playerScore);

		}

		// ������ ������ϰų� �����ϴ� ������ �߰�
	}
}

void ABombGameMode::StartGame()
{
	ShowMessage(TEXT("������ ���۵˴ϴ�. 10�� �� ��ź�� �������� �����ڿ��� �����˴ϴ�."));

	// ��� ĳ������ �Է� ��Ȱ��ȭ
	for (AController* Controller : PlayerControllers)
	{
		if (Controller)
		{
			ADefaultController* PlayerController = Cast<ADefaultController>(Controller);
			if (PlayerController)
			{
				PlayerController->DisableInput(PlayerController);
			}
		}
	}

	// ���� ���� ����
	GetWorldTimerManager().SetTimer(SpawnCharacterTimerHandle, this, &ABombGameMode::RandomSpawn, 10.0f, false);
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
				// UI ���� ���� �� �޽��� ǥ��
				UBombGameStartMessage* MessageWidget = CreateWidget<UBombGameStartMessage>(PlayerController->GetWorld(), MessageWidgetClass);
				if (MessageWidget)
				{
					MessageWidget->InputText = Message;
					MessageWidget->AddToViewport();

					PlayerController->GetWorldTimerManager().SetTimer(WidgetTimerHandle, [PlayerController, MessageWidget]()
						{
							MessageWidget->RemoveFromViewport();
							PlayerController->EnableInput(PlayerController);
						}, 10.0f, false);
				}
			}
		}
	}
}

void ABombGameMode::SetHolderController(ADefaultController* NewController)
{
	BombHolderController = NewController;
}
