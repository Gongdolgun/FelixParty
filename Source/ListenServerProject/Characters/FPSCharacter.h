#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "Misc/Structures.h"
#include "FPSCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AFPSCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

public:
	AFPSCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Hit(AActor* InActor, const FHitData& InHitData) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void Action() override;
	virtual void End_Action() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Run;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHP = 100;

	UPROPERTY(BlueprintReadWrite, Replicated)
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Respawn_time = 3.f;

	UPROPERTY(BlueprintReadWrite)
	bool isRun;

	UPROPERTY(BlueprintReadWrite)
	bool isAim;

private:
	FTimerHandle RespawnTimer;

public:
	void SeperateServer(FWeaponData WeaponData, FHitData HitData);
	void LineTrace(FWeaponData WeaponData, FHitData HitData);

	UFUNCTION(Server, Reliable)
	void LineTrace_Server(FWeaponData WeaponData, FHitData HitData);

	UFUNCTION(NetMulticast, Reliable)
	void FireEvent_NMC(FVector direction, FHitResult HitResult);

	UFUNCTION(NetMulticast, Reliable)
	void Dead_NMC();

	UFUNCTION(Server, Reliable)
	void SetSpeed_Server(float InSpeed);

	UFUNCTION(NetMulticast, Reliable)
	void SetSpeed_NMC(float InSpeed);

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float InSpeed, bool InIsRun);

	void RespawnCharacter();
};
