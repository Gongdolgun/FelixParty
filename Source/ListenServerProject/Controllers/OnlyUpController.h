#pragma once

#include "CoreMinimal.h"
#include "Controllers/DefaultController.h"
#include "Misc/Enums.h"
#include "OnlyUpController.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AOnlyUpController : public ADefaultController
{
	GENERATED_BODY()

public:
	AOnlyUpController();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnPossess(APawn* InPawn) override;

public:
	FString EnumToString(EGameStateType InGameStateType);

	void SetHUDTime();
	void SetGameStateType();


protected:

private:
	UPROPERTY()
	class ADefaultHUD* OnlyUpHUD;

	UPROPERTY()
	class ADefaultGameState* OnlyUpGameState;

private:
	float TimeLeft;

};
