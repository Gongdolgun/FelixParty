#include "StopWatchGameMode.h"
#include "Global.h"
#include "Controllers/StopWatchController.h"
#include "Widgets/StartTimer.h"

AStopWatchGameMode::AStopWatchGameMode()
{
	bReplicates = true;

}

void AStopWatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	RandomTimer();

	if (StartTimerWidgetClass)
	{
		StartTimerWidget = CreateWidget<UStartTimer>(GetWorld(), StartTimerWidgetClass);
		if (StartTimerWidget)
		{
			StartTimerWidget->AddToViewport();
			StartTimer();
		}
	}
}

void AStopWatchGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bGameActive)
	{
		ElapsedTime += DeltaSeconds; // 경과 시간 업데이트
	}
}

void AStopWatchGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ADefaultController* Controller = Cast<ADefaultController>(NewPlayer);

	if (Controller)
	{
		PlayerControllers.Add(Controller);
	}
}

void AStopWatchGameMode::StartTimer()
{
	ElapsedTime = 0.0f; // 경과 시간 초기화
	bGameActive = true; // 게임 활성화 상태

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AStopWatchGameMode::TimerEnd, RandomTime, false);
	StartTimerWidget->UpdateTimer(RandomTime); // 초기 타이머 업데이트
}


void AStopWatchGameMode::StopTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	bGameActive = false; // 게임 비활성화

}

void AStopWatchGameMode::PauseTimer()
{

}

void AStopWatchGameMode::RandomTimer()
{
	RandomTime = FMath::RandRange(10, 30);
	CLog::Log(RandomTime);
}

void AStopWatchGameMode::TimerEnd()
{
	StopTimer();
}

void AStopWatchGameMode::CheckResult(float StopdTime)
{
	if (!bGameActive) return; // 게임이 활성화되어 있지 않으면 반환

	float Difference = FMath::Abs(RandomTime - StopdTime);
	if (Difference <= 2.0f)
	{
		CLog::Print("win");
	}
	else
	{
		CLog::Print("loss");
		
	}
}

void AStopWatchGameMode::MultiStartTimer_Implementation(float Duration)
{

}

void AStopWatchGameMode::MultiStopTimer_Implementation()
{

}

void AStopWatchGameMode::MultiPauseTimer_Implementation()
{

}

