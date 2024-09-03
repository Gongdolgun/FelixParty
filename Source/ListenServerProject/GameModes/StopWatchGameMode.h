#pragma once

#include "CoreMinimal.h"
#include "Characters/StopWatchCharacter.h"
#include "GameModes/DefaultGameMode.h"
#include "Widgets/StartTimer.h"
#include "StopWatchGameMode.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AStopWatchGameMode : public ADefaultGameMode
{
	GENERATED_BODY()

public:
	AStopWatchGameMode();

protected:
	void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

	void OnPostLogin(AController* NewPlayer) override;

public:
	void StartTimer();

	void StopTimer();

	void CheckResult(float StopTime);

	UFUNCTION(NetMulticast, Reliable)
	void MultiUpdateTimer(float UpdateTime);

private:
	TArray<class ADefaultController*> PlayerControllers;

public:
	float ElapsedTime; // ��� �ð�

	bool bGameActive; // ���� Ȱ��ȭ ����

	TMap<AController*, float> PlayerRandomTimes;

	FTimerHandle TimerHandle;

	FTimerHandle RandomTimerHandle;

};
