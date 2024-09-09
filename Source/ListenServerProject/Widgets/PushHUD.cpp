#include "Widgets/PushHUD.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"

void APushHUD::DrawHUD()
{
	Super::DrawHUD();

}

void APushHUD::BeginPlay()
{
	Super::BeginPlay();


}

void APushHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void APushHUD::CreateHPBar()
{
	// HP Widget
	if (SelectHPBar)
	{
		APlayerController* OwningPlayer = GetOwningPlayerController();
		if (OwningPlayer == nullptr) return;

		HpWidget = CreateWidget<UUserWidget>(OwningPlayer, SelectHPBar);
		if (HpWidget)
		{
			HpWidget->AddToViewport();
			HpWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}

}

void APushHUD::OnGamePlayStart(EGameStateType InPrevGameType, EGameStateType InNewGameType)
{
	if (InNewGameType == EGameStateType::GameStart)
	{
		CreateHPBar();
	}
}


