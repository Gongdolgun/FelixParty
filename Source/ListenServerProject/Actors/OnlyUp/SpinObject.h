#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator RotationSpeed;
};
