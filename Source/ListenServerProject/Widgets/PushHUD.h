#pragma once

#include "CoreMinimal.h"
#include "Widgets/DefaultHUD.h"
#include "PushHUD.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API APushHUD : public ADefaultHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

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



