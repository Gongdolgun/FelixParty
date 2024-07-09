#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Structures.h"
#include "Weapon.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName AttachSocketName;

private:
	UPROPERTY(EditAnywhere)
	class USceneComponent* Root;

protected:
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* Mesh;

public:	
	AWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
	virtual void Begin_Equip();
	virtual void End_Equip();

	virtual void Equip();
	virtual void Fire();

	void UnEquip();

protected:
	class ACharacter* Owner;
	FWeaponData WeaponData;

public:

	UPROPERTY(EditAnywhere, Category = "Settings")
	UDataTable* WeaponDataTable;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FName WeaponName;

	UPROPERTY(EditAnywhere, Category = "Settings")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, Category = "HitData")
	FHitData HitData;
};
