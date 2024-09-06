#pragma once

#include "CoreMinimal.h"
#include "Controllers/DefaultController.h"
#include "INHController.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AINHController : public ADefaultController
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(Client, Reliable)
	void PlayHitAnimation();

public:
	UPROPERTY(BlueprintReadWrite)
	class UINHMain* INHMain;
};