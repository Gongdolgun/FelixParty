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
	PrimaryActorTick.bCanEverTick = true;
	bAutoManageActiveCameraTarget = false;

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
	DOREPLIFETIME(ThisClass, PlayerInfo);
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

	ALobbyCharacter* character = Cast<ALobbyCharacter>(GetPawn());
	if (character && Ready_Montages.Num() > 0)
	{
		int32 randomInt = UKismetMathLibrary::RandomIntegerInRange(1, Ready_Montages.Num() - 1);

		// 기본 상태
		if (PlayerInfo.IsReady == false)
		{
			character->PlayReadyMontage(Ready_Montages[0]);
		}
		// 레디하셈
		else
		{
			character->PlayReadyMontage(Ready_Montages[randomInt]);
		}
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
		SetViewTargetWithBlend(LobbyCamera, 0.0f);
	}
}

void ALobbyController::LeaveToMainMenu_Implementation()
{
	UGameplayStatics::OpenLevel(this, FName("MainMenu"));
}

void ALobbyController::LeaveSession_Server_Implementation()
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());

	if(LobbyGameMode != nullptr)
	{
		LobbyGameMode->LeaveSession(this);
	}

	LeaveToMainMenu();
}
