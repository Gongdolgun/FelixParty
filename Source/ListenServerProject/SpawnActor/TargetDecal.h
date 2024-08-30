#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "TargetDecal.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ATargetDecal : public ADecalActor
{
	GENERATED_BODY()

public:
	ATargetDecal();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
