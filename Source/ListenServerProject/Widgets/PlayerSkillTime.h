#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "PlayerSkillTime.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UPlayerSkillTime : public UUserWidget
{
	GENERATED_BODY()

public:
	// 스킬 쿨다운을 업데이트하는 함수 (Blueprint로 구현)
	UFUNCTION(BlueprintImplementableEvent, Category = "Cooldown")
	void UpdateWallCooldown(float CooldownPercent);  // Wall 스킬 쿨다운 업데이트

	UFUNCTION(BlueprintImplementableEvent, Category = "Cooldown")
	void UpdateRestraintCooldown(float CooldownPercent);  // Restraint 스킬 쿨다운 업데이트

public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UImage* WallCooldownIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* RestraintCooldownIcon; 

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* WallCooldownBar; 

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* RestraintCooldownBar; 
};
