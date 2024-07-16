#include "Widgets/OnlyUpHUD.h"

#include "CharacterOverlay.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "GameModes/OnlyUpGameMode.h"

void AOnlyUpHUD::DrawHUD()
{
	Super::DrawHUD();


}

void AOnlyUpHUD::BeginPlay()
{
	Super::BeginPlay();

	AddCharacterOverlay();

	
}

void AOnlyUpHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

void AOnlyUpHUD::AccessGameMode()
{
	OnlyUpGameMode = Cast<AOnlyUpGameMode>(GetWorld()->GetAuthGameMode());
}
