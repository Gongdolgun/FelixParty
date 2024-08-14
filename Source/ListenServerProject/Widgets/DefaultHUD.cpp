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

	if(CharacterOverlayClasses.Num() > 0)
		AddCharacterOverlay(CharacterOverlayClasses[0]);
}

void ADefaultHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ADefaultHUD::AddCharacterOverlay(TSubclassOf<class UUserWidget> InCharacterOverlay)
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController == nullptr) return;

	if (InCharacterOverlay)
	{
		if (CharacterOverlay != nullptr)
			CharacterOverlay->RemoveFromParent();

		CharacterOverlay = CreateWidget<UUserWidget>(PlayerController, InCharacterOverlay);

		if(CharacterOverlay != nullptr)
			CharacterOverlay->AddToViewport();
	}
}

void ADefaultHUD::ChangeWidgetClass(EGameStateType InPrevGameType, EGameStateType InNewGameType)
{
	switch (InNewGameType)
	{
	case EGameStateType::GameStart:
		if(CharacterOverlayClasses.Num() >= 2)
			AddCharacterOverlay(CharacterOverlayClasses[1]);
		break;

	case EGameStateType::GameOver:
		if (CharacterOverlayClasses.Num() >= 3)
			AddCharacterOverlay(CharacterOverlayClasses[2]);
		break;
	}
}