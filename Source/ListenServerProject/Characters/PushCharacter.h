#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "PushCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API APushCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStateComponent* StateComponent;

public:
	APushCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void Action() override;

	virtual void Hit(AActor* InActor, const FHitData& InHitData) override;
	void Dead();

public:
	UFUNCTION(Server, Reliable)
	void SpawnObject_Server(UClass* InClass, FTransform InTransform);

private:
	UFUNCTION(NetMulticast, Reliable)
	void PlayAttackMontage_NMC();

	UFUNCTION(Server, Reliable)
	void PlayAttackMontage_Server();


private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* AttackMontage;
	
};
