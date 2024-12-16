#include "Controllers/CrashController.h"
#include "GameState/DefaultGameState.h"

ACrashController::ACrashController()
{
}

void ACrashController::BeginPlay()
{
	Super::BeginPlay();

    ADefaultGameState* GameState = GetWorld()->GetGameState<ADefaultGameState>();
    if (GameState)
    {
        GameState->OnGameStateTypeChanged.AddDynamic(this, &ACrashController::OnGameStateTypeChanged);
    }
}

void ACrashController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACrashController::CreateCountWidget_Implementation()
{
    if (IsLocalController() && CountUpdateClass)
    {
        CountWidget = CreateWidget<UCountUpdate>(GetWorld(), CountUpdateClass);
        if (CountWidget)
        {
            CountWidget->AddToViewport();
        }
    }
}

void ACrashController::OnGameStateTypeChanged(EGameStateType InPrevGameType, EGameStateType InNewGameType)
{
    if (InNewGameType == EGameStateType::GameStart)
    {
        CreateCountWidget();
    }
}

void ACrashController::CountUpdate_NMC_Implementation(int32 CurrentCount, int32 MaxCount)
{
    WDG_CountUpdate(CurrentCount, MaxCount);
}

void ACrashController::WDG_CountUpdate(int32 CurrentCount, int32 MaxCount)
{
    if (CountWidget)
    {
        CountWidget->UpdateCount(CurrentCount, MaxCount);
    }
}
