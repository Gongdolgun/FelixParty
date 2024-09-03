#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropObject.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADropObject : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Root")
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = "Static Mesh")
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* Collision;
	
public:	
	ADropObject();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void StartSwing();
	void Swing();
	void DropObject();

	void RespawnOriginialLocation();

private:
	UPROPERTY(EditAnywhere, Category = "Value")
	float MaxSwingAmount = 5.0f; // 최대 Roll 각도

	UPROPERTY(EditAnywhere, Category = "Value")
	float SwingSpeed = 400.0f; // 흔들림 속도

	FVector OriginalLocation;
	FRotator OriginalRotation;

	FTimerHandle SwingTimerHandle;
	FTimerHandle DropTimerHandle;



};
