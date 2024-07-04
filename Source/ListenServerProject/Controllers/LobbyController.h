#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "GameFramework/PlayerController.h"
#include "LobbyController.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ALobbyController : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyController();
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ChangeCharacter(TSubclassOf<ADefaultCharacter> NewCharacter);
};
