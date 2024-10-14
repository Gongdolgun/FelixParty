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

	// Option
	UPROPERTY(EditAnywhere, Category = "HUD Types")
	TMap<EOptionTypes, TSubclassOf<class UUserWidget>> OptionWidgetClasses;

	UPROPERTY()
	TMap<EOptionTypes, UUserWidget*> OptionMap;

	// HitAnim

	UPROPERTY(EditAnywhere, Category = "HUD Types")
	TMap<EHitAnimType, TSubclassOf<class UUserWidget>> HitAnimClasses;

	UPROPERTY()
	TMap<EHitAnimType, UUserWidget*> HitAnimMap;

public:
	UUserWidget* CharacterOverlay;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void AddCharacterOverlay(TSubclassOf<class UUserWidget> InCharacterOverlay);

	// Option
	template <typename SelectWidget, typename EnumType>
	void CreateWidgets(TMap<EnumType, TSubclassOf<UUserWidget>>& OutWidgetClasses
					  ,TMap<EnumType, UUserWidget*>& OutputMap);

public:
	UFUNCTION()
	void ChangeWidgetClass(EGameStateType InPrevGameType, EGameStateType InNewGameType);

	void ShowOptionWidget(EOptionTypes InOptionType);

	// Hit Blood
	void PlayHitAnim(EHitAnimType InHitAnimType);

};

template <typename SelectWidget, typename EnumType>
void ADefaultHUD::CreateWidgets(TMap<EnumType, TSubclassOf<UUserWidget>>& OutWidgetClasses,
	TMap<EnumType, UUserWidget*>& OutputMap)
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController) return;

	for (const auto& WidgetPair : OutWidgetClasses)
	{
		EnumType WidgetType = WidgetPair.Key;
		const TSubclassOf<SelectWidget>& WidgetClass = WidgetPair.Value;

		if (WidgetClass)
		{
			if (SelectWidget* widget = CreateWidget<SelectWidget>(PlayerController, WidgetClass))
				OutputMap.Add(WidgetType, widget);
		}
	}
}