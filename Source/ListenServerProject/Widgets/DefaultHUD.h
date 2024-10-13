#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameState/DefaultGameState.h"
#include "Misc/Enums.h"
#include "DefaultHUD.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADefaultHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "Hud Types")
	TMap<EHudTypes, TSubclassOf<class UUserWidget>> HUDClasses;

	UUserWidget* CharacterOverlay;

	UPROPERTY(EditAnywhere, Category = "Hud Types")
	TMap<EOptionTypes, TSubclassOf<class UUserWidget>> OptionClasses;

	UPROPERTY()
	TMap<EOptionTypes, UUserWidget*> OptionWidgets;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void AddCharacterOverlay(TSubclassOf<class UUserWidget> InCharacterOverlay);

	// Option
	void CreateOptionWidgets();
	void ShowOptionWidget(EOptionTypes InOptionType);

public:
	UFUNCTION()
	void ChangeWidgetClass(EGameStateType InPrevGameType, EGameStateType InNewGameType);

};