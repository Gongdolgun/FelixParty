#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Misc/Enums.h"
#include "DefaultGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameStateTypeChanged, EGameStateType, InPrevGameType, EGameStateType, InNewGameType);

UCLASS()
class LISTENSERVERPROJECT_API ADefaultGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ADefaultGameState();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetTimer(float InTime);
	virtual void SetScore() {};

public:
	FGameStateTypeChanged OnGameStateTypeChanged;


public:
	// Time
	UPROPERTY(EditAnywhere, Replicated, Category = "Game State")
	float GameStartTime = 5.0f;

	UPROPERTY(EditAnywhere, Replicated, Category = "Game State")
	float GameMatchTime = 10.0f;

	UPROPERTY(EditAnywhere, Replicated, Category = "Game State")
	float GameOverTime = 5.0f;

protected:
	// Score
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Game State")
	int32 Score = 0;

	// Game State
	UPROPERTY(Replicated, BlueprintReadOnly)
	EGameStateType GameStateType;


public:
	UFUNCTION(BlueprintCallable)
	void SetGameState(EGameStateType InGameStateType);

	UFUNCTION(BlueprintCallable)
	EGameStateType GetGameStateType();

private:
	void ChangeGameType(EGameStateType InGameType);

};
