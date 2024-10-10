#pragma once

#include "CoreMinimal.h"
#include "Characters/BombCharacter.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Widgets/CountDown.h"
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
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Particle;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> CountdownWidgetClass;

	ABombCharacter* Owner;

public:
	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere)
	UAudioComponent* Audio;

	UPROPERTY(EditAnywhere)
	USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* CountDownWidget;

public:
	UPROPERTY(ReplicatedUsing = OnRep_CountdownSound, EditAnywhere)
	USoundBase* CountdownSound;

public:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_UpdateColor, BlueprintReadOnly, Category = "Bomb")
	FVector BombColor;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CountdownSound();

	UFUNCTION()
	void OnRep_UpdateColor();

	UPROPERTY()
	FLinearColor CountdownColor;

	UFUNCTION(NetMulticast, Reliable)
	void MultiStartCountdown();

	UFUNCTION(Server, Reliable)
	void Explosion();

	UFUNCTION(NetMulticast, Reliable)
	void ExplosionEvent(FVector InLocation);

	UFUNCTION(Server, Reliable)
	void CallRespawnbomb();

public:

	void UpDateSoundAndColor(float DeltaTime);

	void UpdateShakeEffect(float DeltaTime);

	void AttachBomb(ABombCharacter* InCharacter);

	UPROPERTY(Replicated)
	float ElapseTime;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float CurrentExplosionTime;

	UPROPERTY(EditAnywhere, Category = "Bomb")
	float ExplosionTime = 30.0f;

	bool bIsExplosion;

	FTimerHandle CountdownTimerHandle;

};

