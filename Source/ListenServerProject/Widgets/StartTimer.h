#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "StartTimer.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UStartTimer : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RandomTimeText;

	void UpdateTimer(float CurrentTime);

	void UpdateRandomTime(float RandomTime);
};
