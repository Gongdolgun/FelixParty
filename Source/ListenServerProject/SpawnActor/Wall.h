#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wall.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AWall : public AActor
{
	GENERATED_BODY()
	
public:	
	AWall();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* Box;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* Niagara;

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

public:

};
