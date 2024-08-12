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
	float SwingAmount = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Value")
	float MaxSwingAmount = 100.0f; // 최대 흔들림 강도

	UPROPERTY(EditAnywhere, Category = "Value")
	float SwingDuration = 2.0f; // 흔드는 시간

	UPROPERTY(EditAnywhere, Category = "Value")
	float SwingElapsedTime = 0.0f; // 경과 시간

	FVector OriginalLocation;

	FTimerHandle SwingTimerHandle;
	FTimerHandle DropTimerHandle;



};
