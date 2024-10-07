#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Enums.h"
#include "SpinObject.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ASpinObject : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Root")
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh1;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh2;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh3;

public:	
	ASpinObject();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(NetMulticast, Reliable)
	void AddRotation_NMC(FRotator InRotationDelta);

	UFUNCTION()
	void OnGamePlayStart(EGameStateType InPrevGameType, EGameStateType InNewGameType);

private:
	UPROPERTY(EditAnywhere, Replicated, Category = "Rotation")
	FRotator RotationSpeed = FRotator(45.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Rotation")
	FRotator RotationDelta;

	UPROPERTY(Replicated)
	bool bCheck = false;

};

