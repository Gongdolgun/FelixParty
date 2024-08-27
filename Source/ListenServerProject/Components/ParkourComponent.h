#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/Enums.h"
#include "Misc/Structures.h"
#include "ParkourComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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
	void CorrectPlayerLocation(EParkourType InParkourType);

	void SetCanParkour(bool bInCanParkour);

public:
	UFUNCTION(BlueprintCallable)
	void ParkourTrace(FParkourStruct InParkourLocation, float InInitialTraceLength, float InSecondaryTraceZOffset, float InFallingHeightMultiplier);

	void ParkourCheck(float InSecondaryTraceZOffset, float InFallingHeightMultiplier, EParkourType InParkourType);

private:
	void LineTrace(EParkourArrowType InType, float InInitialTraceLength);
	bool Check_ObjectRotation();

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
	FParkourRelativeStruct ParkourRelative;

	UPROPERTY(EditAnywhere, Replicated, Category = "Parkour")
	FVector falling_ImpactPoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug_Parkour;

	// TraceTypeQuery1 = Visibility, 3 = Parkour
	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<ETraceTypeQuery> TraceType = TraceTypeQuery1;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parkour")
	UAnimMontage* High_ParkourMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parkour")
	UAnimMontage* Low_ParkourMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parkour")
	UAnimMontage* Jump_ParkourMontage;

	// Arrow Component
	TArray<class UArrowComponent*> Arrows;
	TArray<FHitResult> HitResults;

	UPROPERTY(EditAnywhere, Category = " Parkour")
	float AvailableFrontAngle = 55.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parkour")
	FParkourStruct HighStruct;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parkour")
	FParkourStruct JumpStruct;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parkour")
	FParkourStruct LowStruct;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Parkour")
	FParkourStruct OutParkourStruct;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Parkour")
	FVector CharacterLocation;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Parkour")
	FVector CharacterForward;

public:
	// Play Montage
	UFUNCTION(NetMulticast, Reliable)
	void PlayParkourMontage_NMC(EParkourType ParkourType);

	UFUNCTION(Server, Reliable)
	void PlayParkourMontage_Server(EParkourType ParkourType);

	UFUNCTION(BlueprintCallable)
	void PlayParkourMontage(EParkourType ParkourType);


};