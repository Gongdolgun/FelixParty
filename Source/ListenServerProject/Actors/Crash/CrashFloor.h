#pragma once

#include "CoreMinimal.h"
#include "DestructibleActor.h"
#include "CrashFloor.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ACrashFloor : public ADestructibleActor
{
	GENERATED_BODY()

public:
	ACrashFloor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	class UDestructibleMesh* DestructibleMesh;

	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY()
	FLinearColor FloorColor;

	void LifeCount();

	void ChangeColor();

private:
	int Count;

};
