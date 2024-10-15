#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "CrashCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ACrashCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

public:
	ACrashCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleDefaultsOnly)
	class UZoomComponent* Zoom;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Zoom;

public:
	void Action() override;

	void SetZooming(const FInputActionValue& Value);

};
