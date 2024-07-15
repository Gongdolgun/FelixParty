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
	virtual void Hit(AActor* InActor, const FHitData& InHitData) override;

protected:
	virtual void Action() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHP = 100;

	UPROPERTY(BlueprintReadWrite, Replicated)
	float HP;

public:
	void SerperateServer(FWeaponData WeaponData, FHitData HitData);
	void LineTrace(FWeaponData WeaponData, FHitData HitData);

	UFUNCTION(Server, Reliable)
	void LineTrace_Server(FWeaponData WeaponData, FHitData HitData);

	UFUNCTION(NetMulticast, Reliable)
	void FireEvent_NMC(FVector direction, FHitResult HitResult);

	UFUNCTION(NetMulticast, Reliable)
	void Dead_NMC();
};
