#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/IDamage.h"
#include "INH_AI.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AINH_AI : public ACharacter, public IIDamage
{
	GENERATED_BODY()

public:
	AINH_AI();
	virtual void Hit(AActor* InActor, const FHitData& InHitData) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION(NetMulticast, Reliable)
	void Dead_NMC(FVector InImpulseDirection);

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeadMaterialEvent();
	void DeadMaterialEvent_Implementation();

public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* StunMontage;

};
