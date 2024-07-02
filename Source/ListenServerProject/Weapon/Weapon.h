#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName HolsterSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName RightHandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	class UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	float Montage_PlayRate;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	float HitDistance = 3000;

private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

protected:
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh;

public:	
	AWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Attack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Fire();
	virtual void Fire_Implementation();

public:
	void Equip();

	virtual void Begin_Equip();
	virtual void End_Equip();

protected:
	class AListenServerProjectCharacter* Owner;

};
