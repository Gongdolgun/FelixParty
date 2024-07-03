#include "ListenServerGameMode.h"
#include "Characters/UserPlayer.h"
#include "Global.h"

AListenServerGameMode::AListenServerGameMode()
{
	Helpers::GetClass<APawn>(&DefaultPawnClass, "/Script/Engine.Blueprint'/Game/Player/BP_UserPlayer.BP_UserPlayer_C'");

}
