#include "GameModes/LobbyGameMode.h"
#include "Characters/DefaultCharacter.h"
#include "Controllers/LobbyController.h"
#include "GameFramework/PlayerState.h"
#include "GameInstances/OnlineGameInstance.h"
#include "Global.h"

void ALobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ALobbyController* Controller = Cast<ALobbyController>(NewPlayer);
	UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());

	// �ΰ��ӿ��� ����� �÷��̾� ������ �ʱ�ȭ (Game Instance)
	if(Controller && GameInstance)
	{
		FString PlayerID = FString::FromInt(Controller->GetPlayerState<APlayerState>()->GetUniqueID());

		GameInstance->PlayerDatas.Add(PlayerID, DefaultPlayerData);
	}
}