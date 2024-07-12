#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "LobbyCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ALobbyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALobbyCharacter();

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

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMoveComponent* MoveComponent;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Jump;

public:
	void ChangeMaterial();

	UFUNCTION(Server, Reliable)
	void ChangeMaterial_Server();

	UFUNCTION(NetMulticast, Reliable)
	void ChangeMaterial_NMC(const TArray<UMaterialInterface*>& InMaterials);

	UFUNCTION(Server, Reliable)
	void UpdatePlayer_Server();
};
