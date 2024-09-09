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

public:
	UFUNCTION(Server, Reliable)
	void SpawnObject_Server(UClass* InClass, FTransform InTransform);

private:
	UFUNCTION(NetMulticast, Reliable)
	void PlayActionMontage_NMC(EStateType InStateType);

	UFUNCTION(Server, Reliable)
	void PlayActionMontage_Server(EStateType InStateType);

private:
	UFUNCTION(NetMulticast, Reliable)
	void Dead_NMC(FVector InImpulseDirection);

	UFUNCTION(Server, Reliable)
	void RespawnCharacter_Server();

	UPROPERTY(EditAnywhere, Category = "Respawn")
	TSubclassOf<APushCharacter> RespawnCharacter;

private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* HittedMontage;

	UPROPERTY(EditAnywhere, Category ="Respawn")
	TSubclassOf<AActor> Respawner;
};
