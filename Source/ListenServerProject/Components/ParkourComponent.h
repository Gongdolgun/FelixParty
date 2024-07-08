#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ParkourComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LISTENSERVERPROJECT_API UParkourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UParkourComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ParkourTrace(
		float InitialTraceLength, float SecondaryTraceZOffset,float FallingHeightMultiplier,
		FVector& InParkourPos1, FVector& InParkourPos2);

private:
	ACharacter* OwnerCharacter;

	//FVector ParkourPos1 = FVector::ZeroVector;
	//FVector ParkourPos2 = FVector::ZeroVector;

	//bool CanParkour = false;
};
