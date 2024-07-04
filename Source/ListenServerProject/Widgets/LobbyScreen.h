#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Misc/Structures.h"
#include "LobbyScreen.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ULobbyScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void UpdatePlayerList(const TArray<FPlayerBaseInfo>& PlayerBaseInfos);
	void UpdatePlayerList_Implementation(const TArray<FPlayerBaseInfo>& PlayerBaseInfos);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerBaseInfo> BaseInfos;
};
