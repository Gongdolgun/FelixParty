#include "Actors/OnlyUp/FinishLine.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "Controllers/OnlyUpController.h"
#include "GameModes/OnlyUpGameMode.h"
#include "GameState/OnlyUpGameState.h"

AFinishLine::AFinishLine()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	Helpers::CreateComponent<UBoxComponent>(this, &FinishiLine_Collision, "FinishiLine", RootComponent);

}

void AFinishLine::BeginPlay()
{
	Super::BeginPlay();

	FinishiLine_Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
}

void AFinishLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFinishLine::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (character && (OtherActor != this))
	{
		AOnlyUpGameState* GameState = GetWorld()->GetGameState<AOnlyUpGameState>();
		AOnlyUpGameMode* GameMode = Cast<AOnlyUpGameMode>(GetWorld()->GetAuthGameMode());
		AOnlyUpController* OnlyUpController = Cast<AOnlyUpController>(character->GetController());

		if (OnlyUpController != nullptr && GameState != nullptr && GameMode != nullptr)
		{
			FString PlayerName = OnlyUpController->GetPlayerState<APlayerState>()->GetPlayerName();
			GameState->UpdatePlayerScore(PlayerName, GameState->PlayerRank(OnlyUpController));
			FinishPlayerCount++;

			if (FinishPlayerCount == GameMode->ConnectedPlayers.Num())
			{
				GameState->SetGameState(EGameStateType::GameOver);
			}
		}
	}
}

