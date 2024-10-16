#pragma once

#include "CoreMinimal.h"
#include "Controllers/DefaultController.h"
#include "FinalController.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AFinalController : public ADefaultController
{
	GENERATED_BODY()

public:
	AFinalController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> FinalCamera_Class;

	UPROPERTY(BlueprintReadWrite)
	AActor* FinalCamera;

};
