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
	UFUNCTION(Client, Reliable)
	void CreateOverlayWidget_Client();


	UFUNCTION()
	void OnGamePlay(EGameStateType InPrevGameType, EGameStateType InNewGameType);

public:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> SelectOverlay;

	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	UUserWidget* OverlayWidget;

private:
	ADefaultGameState* DefaultGameState;

};


