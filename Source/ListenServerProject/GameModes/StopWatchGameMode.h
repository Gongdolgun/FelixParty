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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UStartTimer> StartTimerWidgetClass; 

public:
	void StartTimer();

	void StopTimer();

	UFUNCTION(NetMulticast, Reliable)
	void MultiStartTimer();

	UFUNCTION(NetMulticast, Reliable)
	void MultiStopTimer();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRandomTimer();

	UFUNCTION(Server, Reliable)
	void ServerStartTImer();

	UFUNCTION(Server, Reliable)
	void ServerStopTImer();

	UFUNCTION(Server, Reliable)
	void ServerRandomTImer();

	void RandomTimer();

	void TimerEnd();

	void CheckResult(float StopTime);

private:
	TArray<class ADefaultController*> PlayerControllers;

public:
	UStartTimer* StartTimerWidget;

	float ElapsedTime; // ��� �ð�

	bool bGameActive; // ���� Ȱ��ȭ ����

	TMap<AController*, float> PlayerRandomTimes;

	FTimerHandle TimerHandle;

	FTimerHandle RandomTimerHandle;

};
