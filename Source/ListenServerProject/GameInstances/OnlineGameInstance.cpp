#include "GameInstances/OnlineGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "Global.h"

UOnlineGameInstance::UOnlineGameInstance()
{
}

void UOnlineGameInstance::SavePlayerInfo_Implementation(const FString& PlayerID, FPlayerInGameData PlayerData)
{
	if(PlayerDatas.Contains(PlayerID))
		PlayerDatas[PlayerID] = PlayerData;

	CLog::Print(PlayerID);
}
