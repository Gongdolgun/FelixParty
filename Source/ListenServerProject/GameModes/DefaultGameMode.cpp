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
			FBPUniqueNetId PlayerUniqueID = It.Value().UniqueID;
			DefaultGameState->AddPlayerData(PlayerName, 0, PlayerUniqueID);
			/*FString PlayerName = Controller->GetPlayerState<APlayerState>()->GetPlayerName();
			if (GameInstance->PlayerDatas.Contains(PlayerName))
			{
				FBPUniqueNetId PlayerUniqueID = GameInstance->PlayerDatas.Find(PlayerName)->UniqueID;
				DefaultGameState->AddPlayerData(PlayerName, 0, PlayerUniqueID);
			}*/
		}
	}
}

void ADefaultGameMode::UpdatePlayer()
{
	for (auto Player : ConnectedPlayers)
	{
		if (ADefaultCharacter* DefaultCharacter = Cast<ADefaultCharacter>(Player->GetPawn()))
			DefaultCharacter->ChangeMaterial();
	}
}
