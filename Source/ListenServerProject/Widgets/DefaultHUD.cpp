#include "Widgets/DefaultHUD.h"
#include "CharacterOverlay.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameModes/OnlyUpGameMode.h"
#include "Net/UnrealNetwork.h"

void ADefaultHUD::DrawHUD()
{
	Super::DrawHUD();


}

void ADefaultHUD::BeginPlay()
{
	Super::BeginPlay();

	AddCharacterOverlay();

}

void ADefaultHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ADefaultHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController == nullptr) return;

	if (CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}


}



