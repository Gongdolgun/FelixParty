#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DefaultController.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADefaultController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MyUniqueID;
};
