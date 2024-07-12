#include "Controllers/DefaultController.h"
#include "GameInstances/OnlineGameInstance.h"
#include "Global.h"
#include "Characters/DefaultCharacter.h"
#include "Net/UnrealNetwork.h"

void ADefaultController::OnPossess(APawn* InPawn) // 서버에서만 호출
{
	Super::OnPossess(InPawn);

	if (UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance()))
	{
		if (GameInstance->PlayerDatas.Contains(MyUniqueID))
		{
			MyMaterials = GameInstance->PlayerDatas[MyUniqueID].CharacterMaterials;
		}
	}

	if (ADefaultCharacter* MyCharacter = Cast<ADefaultCharacter>(InPawn))
		MyCharacter->UpdatePlayer_Server();
}

void ADefaultController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, MyUniqueID);
	DOREPLIFETIME(ThisClass, MyMaterials);
}