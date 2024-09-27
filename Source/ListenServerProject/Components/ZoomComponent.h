#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZoomComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LISTENSERVERPROJECT_API UZoomComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UZoomComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Zooming")
	float Speed = 60;

	UPROPERTY(EditAnywhere, Category = "Zooming")
	float InterpSpeed = 5;

	UPROPERTY(EditAnywhere, Category = "Zooming")
	FVector2D Range = FVector2D(100, 700);

public:
	void SetZoomValue(float InValue);

private:
	class USpringArmComponent* SpringArm;

	float CurrentValue;
};
