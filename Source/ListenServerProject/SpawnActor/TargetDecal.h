#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "TargetDecal.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ATargetDecal : public ADecalActor
{
	GENERATED_BODY()

public:
	ATargetDecal();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	class UDecalComponent* MouseDecal;

public:
	void UpdateDecalLocation(FVector NewLocation, FRotator NewRotation);

};
