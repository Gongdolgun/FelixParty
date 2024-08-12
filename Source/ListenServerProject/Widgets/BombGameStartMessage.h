#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "BombGameStartMessage.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UBombGameStartMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UTextBlock* MessageTextBlock;

	UPROPERTY(meta = (BindWidget))
	UImage* BackGroundImage; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	FString InputText;

	UPROPERTY(BlueprintReadWrite, Category = "Text")
	FString DisPlayedText;

	UPROPERTY(BlueprintReadWrite, Category = "Text")
	int32 Index = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Text")
	float TypeSpeed = 0.1f;


};
