#include "GameModes/DefaultGameMode.h"
#include "Global.h"
#include "Characters/DefaultCharacter.h"
#include "Controllers/DefaultController.h"

void ADefaultGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (ADefaultController* Controller = Cast<ADefaultController>(NewPlayer))
	{
		ConnectedPlayers.Add(Controller);
		PlayerScores.Add(Controller->GetPlayerState<APlayerState>()->GetPlayerName(), 0);
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

void ADefaultGameMode::UpdatePlayerScore(class ADefaultCharacter InPlayer, int InScore)
{
	ADefaultController* Controller = Cast<ADefaultController>(InPlayer.GetController());

	if(Controller != nullptr)
	{
		FString PlayerName = Controller->GetPlayerState<APlayerState>()->GetPlayerName();

		if(PlayerScores.Contains(PlayerName))
			PlayerScores[PlayerName] += InScore;
	}
}
