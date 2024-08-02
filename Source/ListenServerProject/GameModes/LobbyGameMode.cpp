#include "GameModes/LobbyGameMode.h"
#include "AdvancedSteamFriendsLibrary.h"
#include "OnlineSubsystem.h"
#include "Controllers/LobbyController.h"
#include "GameFramework/PlayerState.h"
#include "GameInstances/OnlineGameInstance.h"
#include "Global.h"
#include "OnlineSubsystem.h"
#include "Characters/LobbyCharacter.h"
#include "Actors/LobbyPlatforms.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Kismet/KismetArrayLibrary.h"

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	//SetUpPlatforms();
	
}

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

		TSharedPtr<const FUniqueNetId> UniqueNetId = Controller->GetPlayerState<APlayerState>()->GetUniqueId().GetUniqueNetId();
		FBPUniqueNetId PlayerUniqueID;
		PlayerUniqueID.SetUniqueNetId(UniqueNetId);

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

	// Looby Spawn
	//UpdatePlayersOnPlatforms();
	
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ALobbyController* lobbyController = Cast<ALobbyController>(Exiting);

	if (lobbyController)
	{
		ConnectedPlayers.Remove(lobbyController);
	}

	//UpdatePlayersOnPlatforms();
}

void ALobbyGameMode::UpdatePlayerLists()
{
	PlayerBaseInfos.Empty();

	for(auto Player : ConnectedPlayers)
		PlayerBaseInfos.Add(Player->PlayerInfo);

	for (auto Player : ConnectedPlayers)
		Player->UpdatePlayerList(PlayerBaseInfos);
}

void ALobbyGameMode::SetUpPlatforms()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Platforms_Class, OutActors);

	for (int32 i = 0; i < OutActors.Num(); i++)
	{
		FName nameTag = FName(*FString::Printf(TEXT("%d"), i));
		//CLog::Print(i, -1, 10, FColor::Red);
		CLog::Print(nameTag.ToString());
		TArray<AActor*> PlatformOutActors;
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), Platforms_Class, nameTag, PlatformOutActors);
		LobbyPlatforms.Add(Cast<ALobbyPlatforms>(PlatformOutActors[0]));

		CLog::Print(LobbyPlatforms[0]);
	}

	bPlatformSetUpDone = true;
}

void ALobbyGameMode::UpdatePlayersOnPlatforms()
{
	bool bLoopPCAllReadyAdded = false;
	PlatformsSetUpIsDone();

	for (ALobbyController* Players : ConnectedPlayers)
	{
		bLoopPCAllReadyAdded = false;

		for (ALobbyPlatforms* Platforms : LobbyPlatforms)
		{
			if (Platforms->LobbyController == Players)
			{
				bLoopPCAllReadyAdded = true;

				break;
			}
		}

		// Completed
		if (bLoopPCAllReadyAdded == false)
		{
			for (ALobbyPlatforms* Platforms : LobbyPlatforms)
			{
				if (Platforms->LobbyController == nullptr)
				{
					Platforms->SpawnCharacter(Players);

					break;
				}
			}
		}
		
	}

	// Completed
	for (auto Platforms : LobbyPlatforms)
	{
		if (ConnectedPlayers.Contains(Platforms->LobbyController) == false)
		{
			Platforms->ClearCharacter();
		}
	}

}

void ALobbyGameMode::PlatformsSetUpIsDone()
{
	if (bPlatformSetUpDone == false)
	{
		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(
			DelayHandle, this, &ThisClass::PlatformsSetUpIsDone, 0.2f, false);
	}

	return;
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
