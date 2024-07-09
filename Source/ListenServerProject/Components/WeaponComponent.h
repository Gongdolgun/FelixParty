#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Misc/Enums.h"
#include "WeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, PrevType, EWeaponType, NewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LISTENSERVERPROJECT_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	ACharacter* Owner;

public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsGunMode() { return Type == EWeaponType::Gun; }


public:	
	UWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	void Begin_Fire();
	void End_Fire();

public:
	void ChangeType(TSubclassOf<class AWeapon> NewWeapon);
	void SetCurrentWeapon(class AWeapon* NewWeapon);

public:
	EWeaponType Type = EWeaponType::Max;	

public:
	FWeaponTypeChanged OnWeaponTypeChange;

public:
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<class AWeapon> StartWeapon;

	UPROPERTY(EditAnywhere)
	AWeapon* CurWeapon;

};
