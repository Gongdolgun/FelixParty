#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Misc/Structures.h"
#include "DefualtSaveGame.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UDefualtSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveSettings")
	FSaveSettings SaveSettings;
	
};
