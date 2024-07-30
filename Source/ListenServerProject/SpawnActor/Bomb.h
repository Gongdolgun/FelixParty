#pragma once

#include "CoreMinimal.h"
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
	class USphereComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Particle;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> CountdownWidgetClass;

public:
	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere)
	UAudioComponent* Audio;

	UPROPERTY(EditAnywhere)
	USoundBase* ExplosionSound;

public:
	UPROPERTY(ReplicatedUsing = OnRep_CountdownSound, EditAnywhere)
	USoundBase* CountdownSound;

public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Bomb")
	bool bBombReplicate;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Bomb")
	bool bBombReplicateMovement;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateBombLocation, BlueprintReadOnly, Category = "Bomb")
	FVector BombLocation;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_UpdateColor, BlueprintReadOnly, Category = "Bomb")
	FVector BombColor;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_UpdateBombLocation();

	UFUNCTION()
	void OnRep_CountdownSound();

	UFUNCTION()
	void OnRep_UpdateColor();

	UFUNCTION(NetMulticast, Reliable)
	void Explosion();

	void DestroyBomb();

	void StartCountdown();

	UPROPERTY()
	FLinearColor InitialColor;

	UPROPERTY()
	FLinearColor CountdownColor;

	UFUNCTION(NetMulticast, Reliable)
	void MultiStartCountdown();

public:
	void UpDateSoundAndColor(float DeltaTime);

	void UpdateShakeEffect(float DeltaTime);

	void ResetShakeEffect();

	FTimerHandle CountdownTimerHandle;

	UPROPERTY(Replicated)
	float TotalCountdownTime;

	UPROPERTY(Replicated)
	float ElapseTime;

	UWidgetComponent* CountDownWidget;

	void UpdateCountdownWidget(float DeltaTime);
};
