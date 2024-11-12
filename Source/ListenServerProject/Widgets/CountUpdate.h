#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CountUpdate.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UCountUpdate : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void UpdateCount(int32 CurrentCount, int32 MaxCount);

protected:
    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* CanvasPanel;

    UPROPERTY(meta = (BindWidget))
    UGridPanel* GridPanel; 

    UPROPERTY(meta = (BindWidget))
    UImage* Image; 

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Current_Count; 

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Max_Count;
};
