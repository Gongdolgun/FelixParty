#include "GameState/INHGameState.h"

void AINHGameState::SomeoneDeadEvent(FString InAttackerName, FString InHittedCharacterName)
{
	if (OnSomeoneDead.IsBound())
		OnSomeoneDead.Broadcast(InAttackerName, InHittedCharacterName);
}
