#include "GameModes/DefaultGameMode.h"
#include "Global.h"
#include "Characters/DefaultCharacter.h"
#include "Controllers/DefaultController.h"
#include "GameState/DefaultGameState.h"

void ADefaultGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (ADefaultController* Controller = Cast<ADefaultController>(NewPlayer))
	{
		ConnectedPlayers.Add(Controller);

		ADefaultGameState* DefaultGameState = GetGameState<ADefaultGameState>();
		if(DefaultGameState != nullptr)
		{
			FString PlayerName = Controller->GetPlayerState<APlayerState>()->GetPlayerName();
			DefaultGameState->AddPlayerScore(PlayerName, 0);
		}
	}
}

void ADefaultGameMode::UpdatePlayer()
{
	for (auto Player : ConnectedPlayers)
	{
		if (ADefaultCharacter* DefaultCharacter = Cast<ADefaultCharacter>(Player->GetPawn()))
			DefaultCharacter->ChangeMaterial();
	}
}