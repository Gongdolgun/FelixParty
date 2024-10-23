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

void UOnlineGameInstance::SelectLevel()
{
	if(CurrentRound == TotalRound)	{
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), "servertravel /Game/Levels/FinalMap");
		return;
	}

	int32 RandomInteger = UKismetMathLibrary::RandomIntegerInRange(0, Maps.Num() - 1);
	int32 PlayerCount = PlayerDatas.Num();
	FString SelectedMap = Maps[RandomInteger];

	Maps.RemoveAt(RandomInteger);

	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), "servertravel /Game/Levels/" + SelectedMap + "?playercount" + FString::FromInt(PlayerCount));
}