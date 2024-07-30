#include "GameInstances/OnlineGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "Global.h"
#include "Net/UnrealNetwork.h"

UOnlineGameInstance::UOnlineGameInstance()
{
}

void UOnlineGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void UOnlineGameInstance::SavePlayerInfo_Implementation(const FString& PlayerID, FPlayerData PlayerData)
{
	if(PlayerDatas.Contains(PlayerID))
		PlayerDatas[PlayerID] = PlayerData;
	
}