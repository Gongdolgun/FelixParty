#pragma once

#include "CoreMinimal.h"
#include "GameState/DefaultGameState.h"
#include "INHGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSomeoneDead, FString, InAttackerName, FString, InHittedCharacterName);

UCLASS()
class LISTENSERVERPROJECT_API AINHGameState : public ADefaultGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FSomeoneDead OnSomeoneDead;

public:
	void SomeoneDeadEvent(FString InAttackerName, FString InHittedCharacterName);
};
