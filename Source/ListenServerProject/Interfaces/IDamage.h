#pragma once

#include "CoreMinimal.h"
#include "Misc/Structures.h"
#include "UObject/Interface.h"
#include "IDamage.generated.h"

UINTERFACE(MinimalAPI)
class UIDamage : public UInterface
{
	GENERATED_BODY()
};

class LISTENSERVERPROJECT_API IIDamage
{
	GENERATED_BODY()

public:
	virtual void Hit(AActor* InActor, const FHitData& InHitData) = 0;
};
