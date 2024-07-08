#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
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
	FORCEINLINE bool GetCanParkour() { return bCanParkour; }

	void SetCanParkour(bool bInCanParkour);

public:
	UFUNCTION(BlueprintCallable)
	void ParkourTrace(
		float InInitialTraceLength, float InSecondaryTraceZOffset,float InFallingHeightMultiplier,
		TEnumAsByte<EDrawDebugTrace::Type> InDrawDebugType ,FVector& InParkourPos1, FVector& InParkourPos2);

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bCanParkour = false;

private:
	ACharacter* OwnerCharacter;

	// 초기 추적 거리
	UPROPERTY(EditAnywhere)
	float Initial_Trace_Length = 120.0f;

	// 
	UPROPERTY(EditAnywhere)
	float Trace_Z_Offset = 200.0;

	// 
	UPROPERTY(EditAnywhere)
	float Falling_Height_Multiplier = 0.5f;

	FVector ParkourPos1 = FVector::ZeroVector;
	FVector ParkourPos2 = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug_Parkour;

	// TraceTypeQuery1 = Visibility
	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<ETraceTypeQuery> TraceType = TraceTypeQuery1;

};
