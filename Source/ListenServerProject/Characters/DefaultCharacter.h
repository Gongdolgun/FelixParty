#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "Interfaces/IDamage.h"
#include "DefaultCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADefaultCharacter : public ACharacter, public IIDamage
{
	GENERATED_BODY()

public:
	ADefaultCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	virtual void Hit(AActor* InActor, const FHitData& InHitData) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMoveComponent* MoveComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWeaponComponent* WeaponComponent;

protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Action;

public:
	UFUNCTION(BlueprintCallable)
	virtual void Action();

	UFUNCTION(BlueprintCallable)
	virtual void End_Action();

public:
	UFUNCTION(Server, Reliable)
	void UpdatePlayer_Server();

	UFUNCTION(Server, Reliable)
	void ChangeMaterial_Server();

	UFUNCTION(NetMulticast, Reliable)
	void ChangeMaterial_NMC(const TArray<UMaterialInterface*>& InMaterials);

	void ChangeMaterial();

};
