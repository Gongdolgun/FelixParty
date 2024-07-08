#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "FPSCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AFPSCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

public:
	AFPSCharacter();
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UAnimInstance> AnimInstance;
};
