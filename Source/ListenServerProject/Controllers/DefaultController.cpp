#include "Controllers/DefaultController.h"
#include "GameInstances/OnlineGameInstance.h"
#include "Global.h"
#include "Characters/DefaultCharacter.h"

void ADefaultController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());

	if (GameInstance)
	{
		ADefaultCharacter* MyCharacter = Cast<ADefaultCharacter>(GetPawn());
		if (MyCharacter)
		{
			if (GameInstance->PlayerDatas.Contains(MyUniqueID))
			{
				for (int i = 0; i < GameInstance->PlayerDatas[MyUniqueID].CharacterMaterials.Num(); i++)
				{
					MyCharacter->GetMesh()->SetMaterial(i, GameInstance->PlayerDatas[MyUniqueID].CharacterMaterials[i]);
				}

				return;
			}
			return;
		}
		return;
	}
}
