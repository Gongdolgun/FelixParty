#pragma once

#include "CoreMinimal.h"
#include "Controllers/DefaultController.h"
#include "OnlyUpController.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AOnlyUpController : public ADefaultController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
};
