#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CountDown.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UCountDown : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Countdown")
	void UpdateCountdown(int CountdownValue);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountdownText;
};
