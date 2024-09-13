#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "PlayerSkillTime.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UPlayerSkillTime : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Cooldown")
	void UpdateCooldown(float CooldownTime, float RemainingTime);

public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UImage* WallCooldownIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* RestraintCooldownIcon; 

	UPROPERTY(meta = (BindWidget))
	UImage* WallBackGroundIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* RestraintBackGroundIcon;

};
