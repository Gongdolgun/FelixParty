#include "Widgets/OnlyUpHUD.h"

#include "CharacterOverlay.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameModes/OnlyUpGameMode.h"
#include "Net/UnrealNetwork.h"

void AOnlyUpHUD::DrawHUD()
{
	Super::DrawHUD();


}

void AOnlyUpHUD::BeginPlay()
{
	Super::BeginPlay();

	AddCharacterOverlay();
	
}

void AOnlyUpHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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



