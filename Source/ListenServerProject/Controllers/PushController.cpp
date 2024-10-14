#include "Controllers/PushController.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "GameState/DefaultGameState.h"
#include "Widgets/DefaultHUD.h"

APushController::APushController()
{

}

void APushController::BeginPlay()
{
	Super::BeginPlay();

	//CreateOverlayWidget();
	DefaultGameState = Cast<ADefaultGameState>(UGameplayStatics::GetGameState(this));
	if (DefaultGameState)
	{
		DefaultGameState->OnGameStateTypeChanged.AddDynamic(this, &ThisClass::OnGamePlay);
	}
}

void APushController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APushController::CreateOverlayWidget()
{
	if (SelectOverlay && IsLocalController())
	{
		OverlayWidget = CreateWidget<UUserWidget>(this, SelectOverlay);
		if (OverlayWidget)
		{
			OverlayWidget->AddToViewport();
			OverlayWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void APushController::OnGamePlay(EGameStateType InPrevGameType, EGameStateType InNewGameType)
{
	if (InNewGameType == EGameStateType::GamePlay)
		CreateOverlayWidget();
}

