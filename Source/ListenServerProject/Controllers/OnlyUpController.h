#pragma once

#include "CoreMinimal.h"
#include "Controllers/DefaultController.h"
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
	void SetHUDCountdown(float InCountdownTime);

	void SetHUDTime();

protected:

private:
	UPROPERTY()
	class AOnlyUpHUD* OnlyUpHUD;

	float MatchTime = 120.0f;
	uint32 CountdownInt = 0;

};
