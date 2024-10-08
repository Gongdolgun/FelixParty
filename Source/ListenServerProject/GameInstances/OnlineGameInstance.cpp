#include "GameInstances/OnlineGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "Global.h"
#include "AdvancedFriendsGameInstance.h"
#include "Net/UnrealNetwork.h"

void UOnlineGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void UOnlineGameInstance::DestroySession_Implementation()
{
}

void UOnlineGameInstance::UpdateTotalScore(const FString& InPlayerName, int32 InScore)
{
	if(PlayerDatas.Contains(InPlayerName))
	{
		PlayerDatas[InPlayerName].TotalScore += InScore;
	}
}

void UOnlineGameInstance::SavePlayerInfo_Implementation(const FString& PlayerID, FPlayerData PlayerData)
{
	if(PlayerDatas.Contains(PlayerID))
		PlayerDatas[PlayerID] = PlayerData;
	
}
