#pragma once

#include "CoreMinimal.h"
#include "Controllers/DefaultController.h"
#include "Widgets/CountUpdate.h"
#include "CrashController.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ACrashController : public ADefaultController
{
	GENERATED_BODY()

public:
	ACrashController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category = "CountUpdate")
	TSubclassOf<UCountUpdate> CountUpdateClass;

	UPROPERTY(BlueprintReadOnly, Category = "CountUpdate")
	UCountUpdate* CountWidget;

	UFUNCTION(Client, Reliable)
	void CreateCountWidget();

	UFUNCTION()
	void OnGameStateTypeChanged(EGameStateType InPrevGameType, EGameStateType InNewGameType);

	UFUNCTION(NetMulticast, Reliable)
	void CountUpdate_NMC(int32 CurrentCount, int32 MaxCount);

	void WDG_CountUpdate(int32 CurrentCount, int32 MaxCount);

};
