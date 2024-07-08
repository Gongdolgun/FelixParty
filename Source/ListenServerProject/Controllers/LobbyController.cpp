#include "Controllers/LobbyController.h"
#include "Global.h"
#include "GameFramework/PlayerState.h"
#include "Misc/Structures.h"
#include "GameInstances/OnlineGameInstance.h"
#include "GameModes/LobbyGameMode.h"

ALobbyController::ALobbyController()
{
}

void ALobbyController::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyController::UpdatePlayerList_Implementation(const TArray<FPlayerBaseInfo>& PlayerBaseInfos)
{
}

void ALobbyController::SetReadyStatus_Implementation()
{
	PlayerInfo.IsReady = !PlayerInfo.IsReady;

	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(World->GetAuthGameMode());
		if (LobbyGameMode)
		{
			LobbyGameMode->UpdatePlayerLists();
			return;
		}
	}
}

void ALobbyController::ChangeCharacter_Implementation(TSubclassOf<ADefaultCharacter> NewCharacter)
{
	FTransform SpawnTransform = GetPawn()->GetActorTransform();
	GetPawn()->Destroy();

	FActorSpawnParameters param;
	ADefaultCharacter* SpawnedCharacter = GetWorld()->SpawnActor<ADefaultCharacter>(NewCharacter, SpawnTransform, param);

	if(SpawnedCharacter)
	{
		Possess(SpawnedCharacter);

		// Game Instance�� Player Data ���� ��û �α� �ۼ�
		UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
		if(GameInstance)
		{
			//FString PlayerID = FString::FromInt(GetPlayerState<APlayerState>()->GetUniqueID());
			if(GameInstance->PlayerDatas.Contains(MyUniqueID))
			{
				FPlayerInGameData PlayerData = *GameInstance->PlayerDatas.Find(MyUniqueID);

				PlayerData.CharacterClass = NewCharacter;

				GameInstance->SavePlayerInfo(MyUniqueID, PlayerData);
			}
		}
	}
}

