#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "OnlyUpCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AOnlyUpCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UParkourComponent* ParkourComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UMotionWarpingComponent> MotionWarpComponent;
	
public:
	AOnlyUpCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void Action() override;
	virtual void Jump() override;

protected:
	UFUNCTION(BlueprintCallable)
	void ExecuteParkour(float InZOffset_Hand,float InZOffset_Landing, float InMontageLength);

private:
	UPROPERTY(EditAnywhere, Category = "Parkour")
	float ZOffset_Hand = -50.0f;

	UPROPERTY(EditAnywhere, Category = "Parkour")
	float ZOffset_Landing = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Parkour")
	float Montage_Length = 1.0f;

};
