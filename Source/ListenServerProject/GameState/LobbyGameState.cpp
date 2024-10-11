#include "GameState/LobbyGameState.h"
#include "Global.h"
#include "GameInstances/OnlineGameInstance.h"
#include "Net/UnrealNetwork.h"

ALobbyGameState::ALobbyGameState()
{
}

void ALobbyGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SelectedColors);
	DOREPLIFETIME(ThisClass, MapIndex);
	DOREPLIFETIME(ThisClass, PlayerBaseInfos);
	DOREPLIFETIME(ThisClass, RoundIndex);
}

void ALobbyGameState::SetTotalRound(int32 InRoundNum)
{
	UOnlineGameInstance* OnlineGameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
	if(OnlineGameInstance != nullptr)
	{
		OnlineGameInstance->TotalRound = RoundIndex + 1;
	}
}
