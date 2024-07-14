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

	FMath::RandInit(FDateTime::Now().GetMillisecond());

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
			if (character)
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


void ABombGameMode::SetHolderController(ADefaultController* NewController)
{
	BombHolderController = NewController;
}
