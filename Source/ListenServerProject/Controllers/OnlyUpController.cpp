#include "Controllers/OnlyUpController.h"
#include "PlayerState/OnlyUp_PlayerState.h"

void AOnlyUpController::BeginPlay()
{
	Super::BeginPlay();

	// PlayerState 세팅, 에디터에서 게임모드 오버라이드에서도 가능
	//AOnlyUp_PlayerState* OnlyUpPlayerState = GetPlayerState<AOnlyUp_PlayerState>();
}
