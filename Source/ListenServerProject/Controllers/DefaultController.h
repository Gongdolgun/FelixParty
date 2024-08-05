#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Misc/Enums.h"
#include "DefaultController.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADefaultController : public APlayerController
{
	GENERATED_BODY()

public:
	ADefaultController();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnPossess(APawn* InPawn) override;

public:
	FString EnumToString(EGameStateType InGameStateType);

	void SetHUDTime();
	void SetGameStateType();

	UFUNCTION(Client, Reliable)
	void LeaveSessionInProgress();

private:
	UPROPERTY()
	class ADefaultHUD* DefaultHUD;

	UPROPERTY()
	class ADefaultGameState* DefaultGameState;

private:
	float TimeLeft;

};
