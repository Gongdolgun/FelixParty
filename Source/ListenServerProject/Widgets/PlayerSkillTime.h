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
	// ��ų ��ٿ��� ������Ʈ�ϴ� �Լ� (Blueprint�� ����)
	UFUNCTION(BlueprintImplementableEvent, Category = "Cooldown")
	void UpdateWallCooldown(float CooldownPercent);  // Wall ��ų ��ٿ� ������Ʈ

	UFUNCTION(BlueprintImplementableEvent, Category = "Cooldown")
	void UpdateRestraintCooldown(float CooldownPercent);  // Restraint ��ų ��ٿ� ������Ʈ

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
