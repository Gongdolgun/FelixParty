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
		ElapsedTime += DeltaSeconds; // ��� �ð� ������Ʈ
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
	ElapsedTime = 0.0f; // ��� �ð� �ʱ�ȭ
	bGameActive = true; // ���� Ȱ��ȭ ����

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AStopWatchGameMode::TimerEnd, RandomTime, false);
	StartTimerWidget->UpdateTimer(RandomTime); // �ʱ� Ÿ�̸� ������Ʈ
}


void AStopWatchGameMode::StopTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	bGameActive = false; // ���� ��Ȱ��ȭ

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
	if (!bGameActive) return; // ������ Ȱ��ȭ�Ǿ� ���� ������ ��ȯ

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

