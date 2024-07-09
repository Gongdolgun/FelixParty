#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "BombCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ABombCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

public:
	ABombCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* TopDownSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCamera;


public:
	void Action() override;
};
