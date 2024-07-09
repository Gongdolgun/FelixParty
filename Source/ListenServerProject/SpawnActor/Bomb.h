#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	ABomb();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly)
	class UNiagaraComponent* Niagara;

};
