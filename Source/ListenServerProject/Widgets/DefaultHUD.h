#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameState/DefaultGameState.h"
#include "DefaultHUD.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADefaultHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TArray<TSubclassOf<class UUserWidget>> CharacterOverlayClasses;

	UUserWidget* CharacterOverlay;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void AddCharacterOverlay(TSubclassOf<class UUserWidget> InCharacterOverlay);

public:
	UFUNCTION()
	void ChangeWidgetClass(EGameStateType InPrevGameType, EGameStateType InNewGameType);

};


