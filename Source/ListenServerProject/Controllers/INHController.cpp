#include "Controllers/INHController.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/INHMain.h"

void AINHController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void AINHController::PlayHitAnimation_Implementation()
{
	if (INHMain != nullptr)
		INHMain->PlayHitAnimation();
}
