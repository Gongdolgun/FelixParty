#pragma once

#include "CoreMinimal.h"
#include "PlayerState/DefaultPlayerState.h"
#include "OnlyUp_PlayerState.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AOnlyUp_PlayerState : public ADefaultPlayerState
{
	GENERATED_BODY()

public:
	AOnlyUp_PlayerState();

public:
	//FORCEINLINE int32 GetPlayerScore() { return Player_Score; }

	UFUNCTION(BlueprintCallable)
	void SetPlayerScore(int32 InPlayerScore);

protected:
	UPROPERTY(BlueprintReadOnly)
	int32 Player_Score = 0;
	
};
