#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Misc/Structures.h"
#include "OnlineGameInstance.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UOnlineGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UOnlineGameInstance();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FPlayerInGameData> PlayerDatas;

public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SavePlayerInfo(const FString& PlayerID, FPlayerInGameData PlayerData);

};