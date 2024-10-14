#pragma once

#include "CoreMinimal.h"
#include "Controllers/DefaultController.h"
#include "Widgets/PlayerSkillTime.h"
#include "BombController.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ABombController : public ADefaultController
{
	GENERATED_BODY()

public:
	ABombController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

};
