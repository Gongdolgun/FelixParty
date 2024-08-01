#include "GameModes/DefaultGameMode.h"
#include "Global.h"
#include "Characters/DefaultCharacter.h"
#include "Controllers/DefaultController.h"
#include "GameInstances/OnlineGameInstance.h"
#include "GameState/DefaultGameState.h"

void ADefaultGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (ADefaultController* Controller = Cast<ADefaultController>(NewPlayer))
	{
		ConnectedPlayers.Add(Controller);
	}
}

void ADefaultGameMode::BeginPlay()
{
	Super::BeginPlay();

	ADefaultGameState* DefaultGameState = GetGameState<ADefaultGameState>();
	UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());

	if (DefaultGameState != nullptr && GameInstance != nullptr)
	{
		for (auto It = GameInstance->PlayerDatas.CreateConstIterator(); It; ++It)
		{
			FString PlayerName = It.Value().PlayerName.ToString();
			FColor PlayerColor = It.Value().PlayerColor;
			DefaultGameState->AddPlayerData(PlayerName, 0, PlayerColor);
		}
	}
}

void ADefaultGameMode::UpdatePlayer()
{
	for (auto Player : ConnectedPlayers)
	{
		UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
		if (GameInstance != nullptr)
		{
			FString PlayerID = Player->GetPlayerState<APlayerState>()->GetPlayerName();

			if (GameInstance->PlayerDatas.Contains(PlayerID))
			{
				if (ADefaultCharacter* DefaultCharacter = Cast<ADefaultCharacter>(Player->GetPawn()))
				{
					DefaultCharacter->ChangeMaterial(GameInstance->PlayerDatas[PlayerID].PlayerColor);
				}
			}
		}
	}
}
