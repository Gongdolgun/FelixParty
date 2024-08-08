#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "BombGameStartMessage.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UBombGameStartMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// 타이머 시작 함수
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StartTimer();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void RemoveWidget();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	FString InputText;

protected:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel; // 캔버스 패널

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MessageTextBlock;

public:
    FTimerHandle Handle;

    FTimerHandle TimerHandle;

	UPROPERTY(BlueprintReadWrite, Category = "Text")
    FString DisPlayedText;

	UPROPERTY(BlueprintReadWrite, Category = "Text")
    int32 Index = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Text")
    float TypeSpeed = 0.1f;

	UPROPERTY(BlueprintReadWrite, Category = "Text")
    bool Condition;
};
