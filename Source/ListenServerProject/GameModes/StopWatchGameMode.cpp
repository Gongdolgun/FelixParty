#include "StopWatchGameMode.h"
#include "Global.h"
#include "Controllers/StopWatchController.h"
#include "Widgets/StartTimer.h"

AStopWatchGameMode::AStopWatchGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bGameActive = false;
}

void AStopWatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (StartTimerWidgetClass)
	{
		StartTimerWidget = CreateWidget<UStartTimer>(GetWorld(), StartTimerWidgetClass);
		if (StartTimerWidget)
		{
			StartTimerWidget->AddToViewport();
		}
	}

}

void AStopWatchGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bGameActive)
	{
		ElapsedTime += DeltaSeconds; // ��� �ð� ������Ʈ
		if (StartTimerWidget)
		{
			StartTimerWidget->UpdateTimer(ElapsedTime);
		}
		CLog::Log(ElapsedTime);

	}
}

void AStopWatchGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ADefaultController* Controller = Cast<ADefaultController>(NewPlayer);

	if (Controller)
	{
		PlayerControllers.Add(Controller);

		if (PlayerControllers.Num() == 1)
		{
			GetWorldTimerManager().SetTimer(RandomTimerHandle, this, &AStopWatchGameMode::RandomTimer, 3.0f, false);

		}
	}
}

void AStopWatchGameMode::StartTimer()
{
	bGameActive = true; // ���� Ȱ��ȭ ����
	ElapsedTime = 0.0f; // ��� �ð� �ʱ�ȭ

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AStopWatchGameMode::TimerEnd, 60.0f, false);

}

void AStopWatchGameMode::StopTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	bGameActive = false;

	MultiStopTimer();
}

void AStopWatchGameMode::MultiRandomTimer_Implementation()
{

}

void AStopWatchGameMode::ServerStartTImer_Implementation()
{

}

void AStopWatchGameMode::ServerStopTImer_Implementation()
{

}

void AStopWatchGameMode::ServerRandomTImer_Implementation()
{

}

void AStopWatchGameMode::MultiStartTimer_Implementation()
{
	StartTimer();
}

void AStopWatchGameMode::MultiStopTimer_Implementation()
{
	StopTimer();
}

void AStopWatchGameMode::RandomTimer()
{
	for (ADefaultController* Controller : PlayerControllers)
	{
		if (AStopWatchController* StopWatchController = Cast<AStopWatchController>(Controller))
		{
			StopWatchController->RandomTime = FMath::RandRange(10, 30);
			PlayerRandomTimes.Add(Controller, StopWatchController->RandomTime);
			CLog::Log(StopWatchController->RandomTime);

			if (StartTimerWidget)
			{
				StartTimerWidget->UpdateRandomTime(StopWatchController->RandomTime);
			}
		}
	}


}

void AStopWatchGameMode::TimerEnd()
{
	StopTimer();
}

void AStopWatchGameMode::CheckResult(float StopTime)
{
	if (!bGameActive)
		return;

	TArray<TPair<AController*, float>> Results;

	for (auto& Entry : PlayerRandomTimes)
	{
		float Difference = FMath::Abs(Entry.Value - StopTime);
		Results.Add(TPair<AController*, float>(Entry.Key, Difference));
	}

	// ���̸� �������� ����
	Results.Sort([](const TPair<AController*, float>& A, const TPair<AController*, float>& B)
		{
			return A.Value < B.Value; // ���̰� ���� ������ ����
		});

	// ��� ���
	for (int32 i = 0; i < Results.Num(); ++i)
	{
		CLog::Print(FString::Printf(TEXT("���� %d: %s, ����: %.2f"), i + 1, *Results[i].Key->GetName(), Results[i].Value));
	}
}