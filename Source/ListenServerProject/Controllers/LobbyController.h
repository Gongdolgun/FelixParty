#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Misc/Structures.h"
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
	void ChangeCharacter(const TArray<UMaterialInterface*>& Materials);

	UFUNCTION(BlueprintNativeEvent)
	void UpdatePlayerList(const TArray<FPlayerBaseInfo>& PlayerBaseInfos);
	void UpdatePlayerList_Implementation(const TArray<FPlayerBaseInfo>& PlayerBaseInfos);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetReadyStatus();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MyUniqueID;

	FPlayerBaseInfo PlayerInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerBaseInfo> PlayerInfos;

	TArray<UMaterialInterface*> MyMaterials;
};
