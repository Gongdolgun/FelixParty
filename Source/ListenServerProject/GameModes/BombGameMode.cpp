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

		// ��� �÷��̾ �α����� �� 10�� ���� ���
        if (PlayerControllers.Num() == 1) // ù ��° �÷��̾ �α����� ���
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
		if (lastPlayer)
		{
			CLog::Log(*lastPlayer->GetName());
		}

		// ������ ������ϰų� �����ϴ� ������ �߰�
	}
}

void ABombGameMode::StartGame()
{
	ShowMessage(TEXT("������ ���۵˴ϴ�. 10�� �� ��ź�� �������� �����ڿ��� �����˴ϴ�."));

	// ���� ���� ����
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
				// UI ���� ���� �� �޽��� ǥ��
				UUserWidget* MessageWidget = CreateWidget<UUserWidget>(PlayerController->GetWorld(), MessageWidgetClass);
				if (MessageWidget)
				{
					MessageWidget->AddToViewport();
					// ������ �޽����� �����ϴ� ����� ���� �߰� ����
				}
			}
		}
	}
}

void ABombGameMode::SetHolderController(ADefaultController* NewController)
{
	BombHolderController = NewController;
}
