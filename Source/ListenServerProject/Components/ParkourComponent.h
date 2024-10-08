#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/Enums.h"
#include "Misc/Structures.h"
#include "ParkourComponent.generated.h"


UCLASS(Blueprintable)
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

	UFUNCTION(NetMulticast, Reliable)
	void CorrectPlayerLocation_NMC(EParkourType InParkourType);

	UFUNCTION(Server, Reliable)
	void CorrectPlayerLocation_Server(EParkourType InParkourType);

	UFUNCTION(BlueprintCallable)
	void CorrectPlayerLocation(EParkourType InParkourType);

	void SetCanParkour(bool bInCanParkour);

public:
	UFUNCTION(BlueprintCallable)
	void ParkourTrace(FParkourStruct InParkourLocation, float InInitialTraceLength, float InSecondaryTraceZOffset, float InFallingHeightMultiplier);

	UFUNCTION(NetMulticast, Reliable)
	void ParkourCheck_NMC(float InSecondaryTraceZOffset, float InFallingHeightMultiplier, EParkourType InParkourType);

	UFUNCTION(Server, Reliable)
	void ParkourCheck_Server(float InSecondaryTraceZOffset, float InFallingHeightMultiplier, EParkourType InParkourType);

	void ParkourCheck(float InSecondaryTraceZOffset, float InFallingHeightMultiplier, EParkourType InParkourType);

private:
	void LineTrace(EParkourArrowType InType, float InInitialTraceLength);
	bool Check_ObjectRotation();

public:
	UPROPERTY(BlueprintReadOnly)
	ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
	bool bCanParkour = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parkour")
	float ZOffset_Hand = -180.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parkour")
	float ZOffset_Landing = 30.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parkour")
	float Montage_Length = 1.1f;

	// 초기 추적 거리
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parkour")
	float Initial_Trace_Length = 50.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parkour")
	float Trace_Z_Offset = 120.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parkour")
	float Falling_Height_Multiplier = 0.5f;


private:
	UPROPERTY(EditAnywhere, Category = "Parkour")
	float Correction_Height_Relative = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Parkour")
	float last_TraceAdd1 = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Parkour")
	float last_TraceAdd2 = 90.0f;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Parkour")
	FVector ParkourPos1 = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Parkour")
	FVector ParkourPos2 = FVector::ZeroVector;

private:
	UPROPERTY(EditAnywhere, Category = "Correct")
	FParkourRelativeStruct ParkourRelative;

	UPROPERTY(EditAnywhere, Category = "Parkour")
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

	UPROPERTY(BlueprintReadWrite, Category = "Parkour")
	FParkourStruct ResultStruct;

	UPROPERTY(BlueprintReadOnly, Category = "Parkour")
	FParkourStruct OutParkourStruct;

	UPROPERTY(BlueprintReadOnly, Category = "Parkour")
	FVector CharacterLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Parkour")
	FVector CharacterForward;

	UPROPERTY(BlueprintReadOnly, Category = "Parkour")
	float AddPlayerLocationZ;

public:
	// Play Montage
	UFUNCTION(NetMulticast, Reliable)
	void PlayParkourMontage_NMC(EParkourType ParkourType);

	UFUNCTION(Server, Reliable)
	void PlayParkourMontage_Server(EParkourType ParkourType);

	UFUNCTION(BlueprintCallable)
	void PlayParkourMontage(EParkourType ParkourType);

};