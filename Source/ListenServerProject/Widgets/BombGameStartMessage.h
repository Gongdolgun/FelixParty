#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "BombGameStartMessage.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UBombGameStartMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MessageTextBlock")
	void SetMessage(const FString& NewMessage);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MessageTextBlock;
};
