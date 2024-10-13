#include "GameModes/DefaultGameMode.h"
#include "Global.h"
#include "Actors/Spawner_TotalCharacter.h"
#include "Actors/TotalScoreCharacter.h"
#include "Characters/DefaultCharacter.h"
#include "Controllers/DefaultController.h"
#include "GameInstances/OnlineGameInstance.h"
#include "GameState/DefaultGameState.h"

ADefaultGameMode::ADefaultGameMode()
{
	Helpers::GetClass(&Spawner_TotalCharacter, "/Game/Widgets/TotalRank/BP_Spawner_TotalCharacter.BP_Spawner_TotalCharacter_C");
	
}

void ADefaultGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	FString ParamValue;
	UKismetSystemLibrary::ParseParamValue(OptionsString, FString("playercount"), ParamValue);

	if (!IsDebugging)
	{
		// 플레이어들 접속 확인
		if (ConnectedPlayers.Num() < FCString::Atoi(*ParamValue))
		{
			if (ADefaultController* Controller = Cast<ADefaultController>(NewPlayer))
			{
				ConnectedPlayers.Add(Controller);
			}
		}

		else
		{
			if (ADefaultController* Controller = Cast<ADefaultController>(NewPlayer))
			{
				Controller->LeaveSessionInProgress();
			}
		}
	}

	else
	{
		if (ADefaultController* Controller = Cast<ADefaultController>(NewPlayer))
		{
			ConnectedPlayers.Add(Controller);
		}
	}
}

void ADefaultGameMode::BeginPlay()
{
	Super::BeginPlay();

	ADefaultGameState* DefaultGameState = GetGameState<ADefaultGameState>();
	UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());

	if (DefaultGameState != nullptr && GameInstance != nullptr)
	{
		DefaultGameState->OnGameStateTypeChanged.AddDynamic(this, &ADefaultGameMode::OnGameStateTypeChanged);

		for (auto It = GameInstance->PlayerDatas.CreateConstIterator(); It; ++It)
		{
			FString PlayerName = It.Value().PlayerName.ToString();
			FColor PlayerColor = It.Value().PlayerColor;
			int32 TotalScore = It.Value().TotalScore;
			DefaultGameState->AddPlayerData(PlayerName, 0, PlayerColor, TotalScore);
		}
	}
}

void ADefaultGameMode::UpdatePlayer()
{
	for (auto Player : ConnectedPlayers)
	{
		UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
		if (GameInstance != nullptr)
		{
			FString PlayerID = Player->GetPlayerState<APlayerState>()->GetPlayerName();

			if (GameInstance->PlayerDatas.Contains(PlayerID))
			{
				if (ADefaultCharacter* DefaultCharacter = Cast<ADefaultCharacter>(Player->GetPawn()))
				{
					DefaultCharacter->ChangeMaterial(GameInstance->PlayerDatas[PlayerID].PlayerColor);
				}
			}
		}
	}
}

void ADefaultGameMode::OnGameStateTypeChanged(EGameStateType InPrevGameType, EGameStateType InNewGameType)
{
	if(InNewGameType == EGameStateType::GameOver)
	{
		if(Spawner_TotalCharacter != nullptr)
		{
			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FVector SpawnLocation = FVector(0, 0, -10000);

			ASpawner_TotalCharacter* SpawnedActor = GetWorld()->SpawnActor<ASpawner_TotalCharacter>(Spawner_TotalCharacter, SpawnLocation, FRotator::ZeroRotator, params);
		}
	}
}
