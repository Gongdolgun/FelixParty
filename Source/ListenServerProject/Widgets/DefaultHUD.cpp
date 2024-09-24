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

	if (HUDClasses.Contains(EHudTypes::Ready))
		AddCharacterOverlay(HUDClasses[EHudTypes::Ready]);
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
		if(HUDClasses.Contains(EHudTypes::Countdown))
			AddCharacterOverlay(HUDClasses[EHudTypes::Countdown]);
		break;

	case EGameStateType::GamePlay:
		if (HUDClasses.Contains(EHudTypes::GamePlay))
			AddCharacterOverlay(HUDClasses[EHudTypes::GamePlay]);
		break;

	case EGameStateType::GameOver:
		if (HUDClasses.Contains(EHudTypes::GameOver))
			AddCharacterOverlay(HUDClasses[EHudTypes::GameOver]);
		break;

	case EGameStateType::RankBoard:
		if (HUDClasses.Contains(EHudTypes::RankBoard))
			AddCharacterOverlay(HUDClasses[EHudTypes::RankBoard]);
		break;
	}
}