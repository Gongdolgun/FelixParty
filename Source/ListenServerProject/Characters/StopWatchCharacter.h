#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "Widgets/StartTimer.h"
#include "StopWatchCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AStopWatchCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

public:
	AStopWatchCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UStartTimer> StartTimerWidgetClass;

public:
	void Action() override;

	void StopButton();

	UFUNCTION(Server, Reliable)
	void ServerStopButton();

	UFUNCTION(NetMulticast, Reliable)
	void MultiStopButton();

	void UpdateTimer(float UpdateTime);

	void StartWidgetViewport();

	UFUNCTION(NetMulticast, Reliable)
	void MultiStartWidgetViewport();

public:
	UStartTimer* StartTimerWidget;

	FTimerHandle StartTimerWidgetHandle;

	float ElapsedTime;

	float StopTime;
};
