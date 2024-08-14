#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
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
	void Action() override;

	void StartTimer();
	void StopTimer();

	UFUNCTION(Server, Reliable)
	void ServerStopTimer();

	UFUNCTION(Client, Reliable)
	void MultiStopTimer(float Time);

};


