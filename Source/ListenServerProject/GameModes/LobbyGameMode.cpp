#include "GameModes/LobbyGameMode.h"
#include "Characters/DefaultCharacter.h"
#include "Controllers/LobbyController.h"
#include "GameFramework/PlayerState.h"
#include "GameInstances/OnlineGameInstance.h"
#include "Global.h"

void ALobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ALobbyController* Controller = Cast<ALobbyController>(NewPlayer);
	UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());

	// �ΰ��ӿ��� ����� �÷��̾� ������ �ʱ�ȭ (Game Instance)
	if(Controller && GameInstance)
	{
		//FString PlayerID = FString::FromInt(Controller->GetPlayerState<APlayerState>()->GetUniqueID());

		GameInstance->PlayerDatas.Add(Controller->MyUniqueID, DefaultPlayerData);
	}

	// Player Base Info �ʱ�ȭ
	if(Controller)
	{
		ConnectedPlayers.Add(Controller);

		if(Controller->IsLocalController())
			Controller->PlayerInfo.IsReady = true;

		else
			Controller->PlayerInfo.IsReady = false;

		Controller->PlayerInfo.PlayerName = FName(*FString::FromInt(Controller->GetPlayerState<APlayerState>()->GetUniqueID()));

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
