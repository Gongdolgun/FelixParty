#pragma once

#include "CoreMinimal.h"
#include "Controllers/DefaultController.h"
#include "PushController.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API APushController : public ADefaultController
{
	GENERATED_BODY()

public:
	APushController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void CreateHPBar();

	UFUNCTION()
	void OnGamePlayStart(EGameStateType InPrevGameType, EGameStateType InNewGameType);

private:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> SelectHPBar;

	UUserWidget* HpWidget;

};


