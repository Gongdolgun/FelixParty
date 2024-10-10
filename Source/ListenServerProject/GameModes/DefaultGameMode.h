#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Misc/Structures.h"
#include "DefaultGameMode.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADefaultGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void BeginPlay() override;

public:
	TArray<class ADefaultController*> ConnectedPlayers;

	UPROPERTY(EditAnywhere)
	bool IsDebugging = false;

public:
	void UpdatePlayer();
};