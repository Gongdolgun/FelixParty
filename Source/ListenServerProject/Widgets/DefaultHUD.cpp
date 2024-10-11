#include "Widgets/DefaultHUD.h"
#include "CharacterOverlay.h"
#include "Global.h"
#include "HitBlood.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Controllers/DefaultController.h"
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

	// Option Widget 持失
	CreateWidgets<UUserWidget, EOptionTypes>(OptionWidgetClasses);

	// Hit Widget 持失
	CreateWidgets<UUserWidget, EHitAnimType>(HitAnimClasses);
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

		if (CharacterOverlay != nullptr)
			CharacterOverlay->AddToViewport();
	}
}

template <typename SelectWidget, typename EnumType>
void ADefaultHUD::CreateWidgets(TMap<EnumType, TSubclassOf<UUserWidget>>& InWidgetClasses)
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController == nullptr) return;

	for (const auto& WidgetPair : InWidgetClasses)
	{
		EnumType WidgetType = static_cast<EnumType>(WidgetPair.Key);
		TSubclassOf<SelectWidget> WidgetClass = WidgetPair.Value;

		if (WidgetClass)
		{
			if (SelectWidget* widget = CreateWidget<SelectWidget>(PlayerController, WidgetClass))
			{
				WidgetMap.FindOrAdd(static_cast<int32>(WidgetType)) = widget;
			}
		}
	}
}

void ADefaultHUD::ShowOptionWidget(EOptionTypes InOptionType)
{
	ADefaultController* PlayerController = Cast<ADefaultController>(GetOwningPlayerController());
	if (PlayerController == nullptr) return;

	UUserWidget** FoundWidgetPtr = WidgetMap.Find((int32)InOptionType);

	if (FoundWidgetPtr)
	{
		UUserWidget* FoundWidget = *FoundWidgetPtr;

		if (FoundWidget && !FoundWidget->IsInViewport())
		{
			if (InOptionType == EOptionTypes::EmoteOption)
			{
				if (PlayerController->bPressKey == false)
				{
					PlayerController->bPressKey = true;
					FoundWidget->AddToViewport();
					FoundWidget->SetFocus();

					PlayerController->SetShowMouseCursor(true);
					PlayerController->SetInputMode(FInputModeUIOnly());
				}
			}

			else
			{
				FoundWidget->AddToViewport();
				FoundWidget->SetFocus();

				PlayerController->SetShowMouseCursor(true);
				PlayerController->SetInputMode(FInputModeUIOnly());
			}
		}
	}
}

void ADefaultHUD::ChangeWidgetClass(EGameStateType InPrevGameType, EGameStateType InNewGameType)
{
	switch (InNewGameType)
	{
	case EGameStateType::GameStart:
		if (HUDClasses.Contains(EHudTypes::Countdown))
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

void ADefaultHUD::PlayHitAnim(EHitAnimType InHitAnimType)
{
	ADefaultController* PlayerController = Cast<ADefaultController>(GetOwningPlayerController());
	if (PlayerController == nullptr) return;

	UUserWidget** FoundWidgetPtr = WidgetMap.Find((int32)InHitAnimType);

	if (FoundWidgetPtr)
	{
		UUserWidget* FoundWidget = *FoundWidgetPtr;

		if (FoundWidget)
		{
			if (!FoundWidget->IsInViewport())
			{
				FoundWidget->AddToViewport();
			}

			UHitBlood* hitBlood = Cast<UHitBlood>(FoundWidget);
			hitBlood->PlayHitAnimation();
		}
	}
}
