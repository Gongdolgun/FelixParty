#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Misc/Structures.h"
#include "PlayerCard.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UPlayerCard : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerBaseInfo BaseInfo;
};
