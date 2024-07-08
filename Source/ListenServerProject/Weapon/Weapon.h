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
	FName AttachSocketName;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	float HitDistance = 3000;

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

	virtual void Attack();

public:
	virtual void Begin_Equip();
	virtual void End_Equip();

	virtual void Equip();
	virtual void Fire();

	void UnEquip();

protected:
	class ACharacter* Owner;

};
