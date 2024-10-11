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

private:
	UPROPERTY(EditAnywhere, Category = "HUD Types")
	TMap<EHudTypes, TSubclassOf<class UUserWidget>> HUDClasses;

	UPROPERTY(EditAnywhere, Category = "HUD Types")
	TMap<EOptionTypes, TSubclassOf<class UUserWidget>> OptionWidgetClasses;

	UPROPERTY(EditAnywhere, Category = "HUD Types")
	TMap<EHitAnimType, TSubclassOf<class UUserWidget>> HitAnimClasses;

	UPROPERTY()
	TMap<int32, UUserWidget*> WidgetMap;

public:
	UUserWidget* CharacterOverlay;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void AddCharacterOverlay(TSubclassOf<class UUserWidget> InCharacterOverlay);

	// Option
	template <typename SelectWidget, typename EnumType>
	void CreateWidgets(TMap<EnumType, TSubclassOf<UUserWidget>>& InWidgetClasses);

	void ShowOptionWidget(EOptionTypes InOptionType);

public:
	UFUNCTION()
	void ChangeWidgetClass(EGameStateType InPrevGameType, EGameStateType InNewGameType);

	// Hit Blood
	void PlayHitAnim(EHitAnimType InHitAnimType);

};
