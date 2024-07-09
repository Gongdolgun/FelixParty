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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetParkourPos1();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetParkourPos2();

	void SetCanParkour(bool bInCanParkour);

public:
	UFUNCTION(BlueprintCallable)
	void ParkourTrace(float InInitialTraceLength, float InSecondaryTraceZOffset,float InFallingHeightMultiplier);


protected:
	UPROPERTY(BlueprintReadOnly)
	bool bCanParkour = false;

private:
	ACharacter* OwnerCharacter;

	// 초기 추적 거리
	UPROPERTY(EditAnywhere, Category = "Parkour")
	float Initial_Trace_Length = 120.0f;

	// 
	UPROPERTY(EditAnywhere, Category = "Parkour")
	float Trace_Z_Offset = 200.0f;

	// 
	UPROPERTY(EditAnywhere, Category = "Parkour")
	float Falling_Height_Multiplier = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Parkour")
	float Correction_Height_Relative = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Parkour")
	float last_TraceAdd1 = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Parkour")
	float last_TraceAdd2 = 90.0f;

	FVector ParkourPos1 = FVector::ZeroVector;
	FVector ParkourPos2 = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug_Parkour;

	// TraceTypeQuery1 = Visibility
	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<ETraceTypeQuery> TraceType = TraceTypeQuery1;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parkour")
	UAnimMontage* ParkourMontage;

};

