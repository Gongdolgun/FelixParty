#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Misc/Enums.h"
#include "DefaultGameMode.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADefaultGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

public:
	TArray<class ADefaultController*> ConnectedPlayers;

public:
	void UpdatePlayer();

};
