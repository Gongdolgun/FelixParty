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

	CLog::Print(Controller->GetPlayerState<APlayerState>()->GetUniqueId()->ToString());

	// 인게임에서 사용할 플레이어 데이터 초기화 (Game Instance)
	if(Controller && GameInstance)
	{
		FString PlayerID = Controller->GetPlayerState<APlayerState>()->GetPlayerName();

		/*TSharedPtr<const FUniqueNetId> UniqueNetId = Controller->GetPlayerState<APlayerState>()->GetUniqueId().GetUniqueNetId();
		FBPUniqueNetId PlayerUniqueID;
		PlayerUniqueID.SetUniqueNetId(UniqueNetId);*/

		GameInstance->PlayerDatas.Add(PlayerID, DefaultPlayerData);
		if(GameInstance->PlayerDatas.Contains(PlayerID))
		{
			GameInstance->PlayerDatas[PlayerID].PlayerName = FName(*PlayerID);
		}
	}

	// Player Base Info 초기화
	if(Controller && GameInstance)
	{
		ConnectedPlayers.Add(Controller);

		Controller->PlayerInfo.IsReady = Controller->IsLocalController();

		if (GameInstance->PlayerDatas.Contains(Controller->GetPlayerState<APlayerState>()->GetPlayerName()))
		{
			Controller->PlayerInfo.PlayerName = GameInstance->PlayerDatas[Controller->GetPlayerState<APlayerState>()->GetPlayerName()].PlayerName;
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

void ALobbyGameMode::UpdatePlayerMaterial()
{
	for (auto Player : ConnectedPlayers)
	{
		UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
		if(GameInstance != nullptr)
		{
			FString PlayerID = Player->GetPlayerState<APlayerState>()->GetPlayerName();

			if(GameInstance->PlayerDatas.Contains(PlayerID))
			{
				if (ALobbyCharacter* LobbyCharacter = Cast<ALobbyCharacter>(Player->GetPawn()))
				{
					LobbyCharacter->ChangeMaterial(GameInstance->PlayerDatas[PlayerID].PlayerColor);
				}
			}
		}
	}
}

// 세션 나가기
void ALobbyGameMode::LeaveSession(ALobbyController* InController)
{
	ConnectedPlayers.Remove(InController);
	UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());

	if(GameInstance != nullptr)
	{
		FString PlayerID = InController->GetPlayerState<APlayerState>()->GetPlayerName();
		if(GameInstance->PlayerDatas.Contains(PlayerID))
		{
			GameInstance->PlayerDatas.Remove(PlayerID);
		}

		for(int i = 0; i < PlayerBaseInfos.Num(); i++)
		{
			if (PlayerBaseInfos[i].PlayerName == PlayerID)
				PlayerBaseInfos.RemoveAt(i);
		}

		for(auto Player : ConnectedPlayers)
		{
			Player->UpdatePlayerList(PlayerBaseInfos);
		}
	}
}
