#include "GameModes/DefaultGameMode.h"
#include "Characters/DefaultCharacter.h"
#include "GameFramework/PlayerState.h"
#include "GameInstances/OnlineGameInstance.h"
#include "Global.h"
#include "Controllers/DefaultController.h"

APawn* ADefaultGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

	ADefaultController* Controller = Cast<ADefaultController>(NewPlayer);
	//FString PlayerID = FString::FromInt(NewPlayer->GetPlayerState<APlayerState>()->GetUniqueID());
	UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());

	CLog::Print(Controller->MyUniqueID);

	if(GameInstance)
	{
		if(GameInstance->PlayerDatas.Contains(Controller->MyUniqueID))
		{
			FActorSpawnParameters param;

			return GetWorld()->SpawnActor<ADefaultCharacter>(GameInstance->PlayerDatas[Controller->MyUniqueID].CharacterClass, StartSpot->GetActorTransform(), param);
		}
		CLog::Print("Player Datas is not contains");
		return Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
	}
	CLog::Print("GameInstance is not valid");

	return Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
}
