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
	virtual void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerLists();

public:
	// 로비 게임모드에서 관리할 플레이어 컨트롤러들과 기본 정보들
	UPROPERTY(BlueprintReadWrite)
	TArray<class ALobbyController*> ConnectedPlayers;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerBaseInfo> PlayerBaseInfos;

	// 기본 플레이어 데이터 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerData DefaultPlayerData;

public:
	void UpdatePlayerMaterial();

public:
	// Lobby Spawn
	UFUNCTION(BlueprintCallable)
	void SetUpPlatforms();

	UFUNCTION(BlueprintCallable)
	void UpdatePlayersOnPlatforms();

	void PlatformsSetUpIsDone();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Platforms_Class;

	TArray<class ALobbyPlatforms*> LobbyPlatforms;
	bool bPlatformSetUpDone = false;
};
