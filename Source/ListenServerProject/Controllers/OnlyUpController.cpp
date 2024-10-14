#include "Controllers/OnlyUpController.h"
#include "Global.h"
#include "Characters/OnlyUpCharacter.h"
#include "Components/TextBlock.h"
#include "GameState/OnlyUpGameState.h"
#include "Widgets/CharacterOverlay.h"
#include "Widgets/DefaultHUD.h"

AOnlyUpController::AOnlyUpController()
{

}

void AOnlyUpController::BeginPlay()
{
	Super::BeginPlay();

}

void AOnlyUpController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//
	// ::LogAndPrint(bPressKey);
}

void AOnlyUpController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	
}


