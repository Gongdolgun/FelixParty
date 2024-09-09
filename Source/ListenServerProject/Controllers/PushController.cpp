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

	CreateHPBar();
	//ADefaultGameState* gameState = Cast<ADefaultGameState>(UGameplayStatics::GetGameState(this));
	//if (gameState)
	//{
	//	gameState->OnGameStateTypeChanged.AddDynamic(this, &ThisClass::OnGamePlayStart);
	//}
}

void APushController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APushController::CreateHPBar()
{
	// HP Widget
	if (SelectHPBar && IsLocalController())
	{
		HpWidget = CreateWidget<UUserWidget>(this, SelectHPBar);
		if (HpWidget)
		{
			HpWidget->AddToViewport();
			HpWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void APushController::OnGamePlayStart(EGameStateType InPrevGameType, EGameStateType InNewGameType)
{
	if (InNewGameType == EGameStateType::GameStart)
	{
		CreateHPBar();
	}
}

