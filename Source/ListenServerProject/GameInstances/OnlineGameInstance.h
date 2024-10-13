#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "Engine/GameInstance.h"
#include "Misc/Structures.h"
#include "OnlineGameInstance.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UOnlineGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FPlayerData> PlayerDatas;

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> Maps;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSaveSettings Instance_Settings;

public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SavePlayerInfo(const FString& PlayerID, FPlayerData PlayerData);

	UFUNCTION(BlueprintNativeEvent)
	void DestroySession();
	void DestroySession_Implementation();

	UFUNCTION(BlueprintCallable)
	void UpdateTotalScore(const FString& InPlayerName, int32 InScore);
};