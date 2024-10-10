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

	CreateOptionWidgets();
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

void ADefaultHUD::CreateOptionWidgets()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController == nullptr) return;

	for (const auto& OptionPair : OptionClasses)
	{
		EOptionTypes OptionType = OptionPair.Key;
		TSubclassOf<UUserWidget> WidgetClass = OptionPair.Value;

		if (WidgetClass)
		{
			UUserWidget* OptionWidget = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
			if (OptionWidget)
			{
				OptionWidgets.Add(OptionType, OptionWidget); 
			}
		}
	}
}

void ADefaultHUD::ShowOptionWidget(EOptionTypes InOptionType)
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController == nullptr) return;

	UUserWidget** FoundWidgetPtr = OptionWidgets.Find(InOptionType);

	if (FoundWidgetPtr)
	{
		UUserWidget* FoundWidget = *FoundWidgetPtr;

		if (FoundWidget && !FoundWidget->IsInViewport())
		{
			FoundWidget->AddToViewport();
			FoundWidget->SetFocus();

			PlayerController->SetShowMouseCursor(true);
			PlayerController->SetInputMode(FInputModeGameAndUI());
		}
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

	case EGameStateType::InGameRankBoard:
		if (HUDClasses.Contains(EHudTypes::InGameRankBoard))
			AddCharacterOverlay(HUDClasses[EHudTypes::InGameRankBoard]);
		break;

	case EGameStateType::TotalRankBoard:
		if (HUDClasses.Contains(EHudTypes::TotalRankBoard))
			AddCharacterOverlay(HUDClasses[EHudTypes::TotalRankBoard]);
		break;
	}
}
