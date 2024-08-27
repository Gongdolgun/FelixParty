#include "StopWatchGameMode.h"
#include "Global.h"
#include "Controllers/StopWatchController.h"
#include "Widgets/StartTimer.h"

AStopWatchGameMode::AStopWatchGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	bGameActive = false;

	ElapsedTime = 0.0f;
}

void AStopWatchGameMode::BeginPlay()
{
	Super::BeginPlay();


}

void AStopWatchGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bGameActive)
	{
		ElapsedTime += DeltaSeconds; 

		if (ElapsedTime >= 60.0f)
		{
			ElapsedTime = 60.0f; 
			StopTimer();
		}

		if (HasAuthority())
		{
			MultiUpdateTimer(ElapsedTime);
		}
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
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AStopWatchGameMode::StartTimer, 5.0f, false);
		}
	}
}

void AStopWatchGameMode::StartTimer()
{
	if (HasAuthority())
	{
		bGameActive = true;
		ElapsedTime = 0.0f;

	}
}

void AStopWatchGameMode::StopTimer()
{
	bGameActive = false;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
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

	// 차이를 기준으로 정렬
	Results.Sort([](const TPair<AController*, float>& A, const TPair<AController*, float>& B)
		{
			return A.Value < B.Value; // 차이가 작은 순으로 정렬
		});

	// 결과 출력
	for (int32 i = 0; i < Results.Num(); ++i)
	{
		CLog::Print(FString::Printf(TEXT("순위 %d: %s, 차이: %.2f"), i + 1, *Results[i].Key->GetName(), Results[i].Value));
	}
}

void AStopWatchGameMode::MultiUpdateTimer_Implementation(float UpdateTime)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			AStopWatchCharacter* Character = Cast<AStopWatchCharacter>(PC->GetPawn());
			if (Character)
			{
				Character->UpdateTimer(UpdateTime); // 클라이언트에게 경과 시간 업데이트
			}
		}
	}
}

