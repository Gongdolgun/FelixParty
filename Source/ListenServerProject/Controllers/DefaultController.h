#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DefaultController.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADefaultController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	TArray<UMaterialInterface*> MyMaterials;

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
