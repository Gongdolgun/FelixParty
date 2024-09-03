#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "TargetAim.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UTargetAim : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	class UImage* TargetAimImage;

	UFUNCTION(BlueprintCallable, Category = "CrossHair")
	void UpdateTargetAimPosition(const FVector& Position);

};
