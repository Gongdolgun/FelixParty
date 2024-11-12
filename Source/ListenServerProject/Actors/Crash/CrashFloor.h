#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrashFloor.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ACrashFloor : public AActor
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
	class UDestructibleComponent* DestructibleMesh;

	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY()
	FLinearColor FloorColor;

	void LifeCount();

	void ChangeColor();

private:
	int Count;

};
