#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Misc/Structures.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerBaseInfo> PlayerBaseInfo;

	// �⺻ �÷��̾� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerInGameData DefaultPlayerData;

};
