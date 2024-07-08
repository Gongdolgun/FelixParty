#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "OnlyUpCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AOnlyUpCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParkourComponent* ParkourComponent;

public:
	AOnlyUpCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void Action() override;

};
