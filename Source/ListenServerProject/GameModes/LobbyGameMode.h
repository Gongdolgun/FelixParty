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

	// 기본 플레이어 데이터 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerInGameData DefaultPlayerData;

};
