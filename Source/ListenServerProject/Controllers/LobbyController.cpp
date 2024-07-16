#include "Controllers/LobbyController.h"
#include "Global.h"
#include "Characters/LobbyCharacter.h"
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

void ALobbyController::ChangeCharacter_Implementation(const TArray<UMaterialInterface*>& Materials)
{
	// Game Instance의 Player Data 편집 요청 로그 작성
	UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		if (GameInstance->PlayerDatas.Contains(MyUniqueID))
		{
			FPlayerInGameData PlayerData = *GameInstance->PlayerDatas.Find(MyUniqueID);

			PlayerData.CharacterMaterials = Materials;

			GameInstance->SavePlayerInfo(MyUniqueID, PlayerData);
		}
	}

	MyMaterials = Materials;
	if(ALobbyCharacter* MyCharacter = Cast<ALobbyCharacter>(GetPawn()))
		MyCharacter->ChangeMaterial();
}