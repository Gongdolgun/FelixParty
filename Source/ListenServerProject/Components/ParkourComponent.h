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

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FORCEINLINE bool GetCanParkour() { return bCanParkour; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetParkourPos1();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetParkourPos2();

	UFUNCTION(BlueprintCallable)
	void CorrectPlayerLocation();

	void SetCanParkour(bool bInCanParkour);

public:
	UFUNCTION(BlueprintCallable)
	void ParkourTrace(
		FVector& OutLocation1, FVector& OutLocation2,
		float InInitialTraceLength, float InSecondaryTraceZOffset,float InFallingHeightMultiplier);

protected:
	UPROPERTY(BlueprintReadOnly, Replicated)
	ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bCanParkour = false;

private:
	UPROPERTY(EditAnywhere, Replicated, Category = "Parkour")
	float Correction_Height_Relative = 15.0f;

	UPROPERTY(EditAnywhere, Replicated, Category = "Parkour")
	float last_TraceAdd1 = 90.0f;

	UPROPERTY(EditAnywhere, Replicated, Category = "Parkour")
	float last_TraceAdd2 = 90.0f;

public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Parkour")
	FVector ParkourPos1 = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Parkour")
	FVector ParkourPos2 = FVector::ZeroVector;

private:
	UPROPERTY(EditAnywhere, Replicated, Category = "Correct")
	float AddPlayerLocationForward = -15.0f;

	UPROPERTY(EditAnywhere, Replicated, Category = "Correct")
	float AddPlayerLocationZ = -80.0f;

	UPROPERTY(EditAnywhere, Replicated, Category = "Parkour")
	FVector falling_ImpactPoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug_Parkour;

	// TraceTypeQuery1 = Visibility, 3 = Parkour
	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<ETraceTypeQuery> TraceType = TraceTypeQuery3;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parkour")
	UAnimMontage* ParkourMontage;

};

