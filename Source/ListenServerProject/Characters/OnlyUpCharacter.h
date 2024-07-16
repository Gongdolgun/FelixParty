#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "OnlyUpCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AOnlyUpCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UParkourComponent* ParkourComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UMotionWarpingComponent> MotionWarpComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* ArrowGroup;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TArray<class UArrowComponent*> Arrows;

public:
	AOnlyUpCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void Action() override;
	virtual void Jump() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated, Category = "Parkour")
	float ZOffset_Hand = -60.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated, Category = "Parkour")
	float ZOffset_Landing = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated, Category = "Parkour")
	float Montage_Length = 1.1f;

	// 초기 추적 거리
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated, Category = "Parkour")
	float Initial_Trace_Length = 50.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated, Category = "Parkour")
	float Trace_Z_Offset = 120.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated, Category = "Parkour")
	float Falling_Height_Multiplier = 0.5f;
	

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void PlayParkourMontage_NMC();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void PlayParkourMontage_Server();

	UFUNCTION(BlueprintCallable)
	void PlayParkourMontage();

public:
	// 모드 변경
	UFUNCTION(NetMulticast, Reliable)
	void SetModeAndCollision_NMC(EMovementMode InMovementMode, bool InCollision);

	UFUNCTION(Server, Reliable)
	void SetModeAndCollision_Server(EMovementMode InMovementMode, bool InCollision);

	UFUNCTION(BlueprintCallable)
	void SetModeAndCollision(EMovementMode InMovementMode, bool InCollision);

protected:
	UFUNCTION(BlueprintCallable)
	void PlayParkour(FVector InParkourPos1, FVector InParkourPos2, float InZOffsetHand, float InZOffsetLanding, float InMontageLength);
};
