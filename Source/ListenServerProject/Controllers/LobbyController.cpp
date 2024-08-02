#include "Controllers/LobbyController.h"
#include "Global.h"
#include "Characters/LobbyCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Misc/Structures.h"
#include "GameInstances/OnlineGameInstance.h"
#include "GameModes/LobbyGameMode.h"
#include "Net/UnrealNetwork.h"

ALobbyController::ALobbyController()
{
	PrimaryActorTick.bCanEverTick = false;
	APlayerController::SetIgnoreLookInput(true);
	APlayerController::SetIgnoreMoveInput(true);

	bReplicates = true;
}

void ALobbyController::BeginPlay()
{
	Super::BeginPlay();

	SetViewCamera();
}

void ALobbyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	
}

void ALobbyController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, LobbyCamera);
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
			LobbyGameMode->UpdatePlayerLists();
	}
}

void ALobbyController::ChangeCharacter_Implementation(FColor MaterialColor)
{
	// Game Instance의 Player Data 편집 요청 로그 작성
	UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		if (GameInstance->PlayerDatas.Contains(GetPlayerState<APlayerState>()->GetPlayerName()))
		{
			FPlayerData PlayerData = *GameInstance->PlayerDatas.Find(GetPlayerState<APlayerState>()->GetPlayerName());
			PlayerData.PlayerColor = MaterialColor;

			GameInstance->SavePlayerInfo(GetPlayerState<APlayerState>()->GetPlayerName() , PlayerData);
		}
	}

	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if(LobbyGameMode != nullptr)
	{
		LobbyGameMode->UpdatePlayerMaterial();
	}

	// MyMaterials = Materials;
	/*if(ALobbyCharacter* MyCharacter = Cast<ALobbyCharacter>(GetPawn()))
		MyCharacter->ChangeMaterial(MaterialColor);*/
}

void ALobbyController::SetViewCamera()
{
	LobbyCamera = UGameplayStatics::GetActorOfClass(GetWorld(), LobbyCamera_Class);
	if (LobbyCamera)
	{
		this->SetViewTargetWithBlend(LobbyCamera, 0.0f);

	}
}