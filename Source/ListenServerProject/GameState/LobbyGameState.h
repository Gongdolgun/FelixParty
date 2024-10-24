#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Misc/Structures.h"
#include "LobbyGameState.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ALobbyGameState();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<FString> SelectedColors;

	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 MapIndex;

	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 RoundIndex;   

	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<FPlayerBaseInfo> PlayerBaseInfos;

public:
	UFUNCTION(BlueprintCallable)
	void SetTotalRound(int32 InRoundNum);
};