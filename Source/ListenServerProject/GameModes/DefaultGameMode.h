#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DefaultGameMode.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADefaultGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

public:
	TArray<class ADefaultController*> ConnectedPlayers;

public:
	void UpdatePlayer();
};
