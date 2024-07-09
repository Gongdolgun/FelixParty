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
	virtual void Hit(AActor* InActor, const FHitData& InHitData) override;

protected:
	virtual void Action() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHP = 100;

	UPROPERTY(BlueprintReadOnly, Replicated)
	float HP;
};
