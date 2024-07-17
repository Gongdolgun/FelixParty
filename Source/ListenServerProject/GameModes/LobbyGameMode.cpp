#include "GameModes/LobbyGameMode.h"
#include "AdvancedSteamFriendsLibrary.h"
#include "OnlineSubsystem.h"
#include "Controllers/LobbyController.h"
#include "GameFramework/PlayerState.h"
#include "GameInstances/OnlineGameInstance.h"
#include "Global.h"
#include "OnlineSubsystem.h"
#include "Characters/LobbyCharacter.h"
#include "Interfaces/OnlineFriendsInterface.h"

void ALobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ALobbyController* Controller = Cast<ALobbyController>(NewPlayer);
	UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());

	// 인게임에서 사용할 플레이어 데이터 초기화 (Game Instance)
	if(Controller && GameInstance)
	{
		FString PlayerID = Controller->GetPlayerState<APlayerState>()->GetPlayerName();
		GameInstance->PlayerDatas.Add(PlayerID, DefaultPlayerData);
		if(GameInstance->PlayerDatas.Contains(PlayerID))
		{
			GameInstance->PlayerDatas.Find(PlayerID)->PlayerName = FName(*PlayerID);
			SetUserUniqueID(NewPlayer);
		}
	}

	// Player Base Info 초기화
	if(Controller && GameInstance)
	{
		ConnectedPlayers.Add(Controller);

		if(Controller->IsLocalController())
			Controller->PlayerInfo.IsReady = true;

		else
			Controller->PlayerInfo.IsReady = false;

		if (GameInstance->PlayerDatas.Contains(Controller->GetPlayerState<APlayerState>()->GetPlayerName()))
		{
			Controller->PlayerInfo.PlayerName = GameInstance->PlayerDatas.Find(Controller->GetPlayerState<APlayerState>()->GetPlayerName())->PlayerName;
			Controller->PlayerInfo.UniqueID = GameInstance->PlayerDatas.Find(Controller->GetPlayerState<APlayerState>()->GetPlayerName())->UniqueID;
		}

		PlayerBaseInfos.Add(Controller->PlayerInfo);

		for(auto Player : ConnectedPlayers)
		{
			Player->UpdatePlayerList(PlayerBaseInfos);
		}
	}
}

void ALobbyGameMode::UpdatePlayerLists()
{
	PlayerBaseInfos.Empty();

	for(auto Player : ConnectedPlayers)
		PlayerBaseInfos.Add(Player->PlayerInfo);

	for (auto Player : ConnectedPlayers)
		Player->UpdatePlayerList(PlayerBaseInfos);
}

void ALobbyGameMode::SetUserUniqueID_Implementation(AController* NewPlayer)
{
}

void ALobbyGameMode::UpdatePlayerMaterial()
{
	for (auto Player : ConnectedPlayers)
	{
		if (ALobbyCharacter* LobbyCharacter = Cast<ALobbyCharacter>(Player->GetPawn()))
			LobbyCharacter->ChangeMaterial();
	}
}
