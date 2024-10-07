#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "OnlyUpCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AOnlyUpCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UParkourComponent* ParkourComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UMotionWarpingComponent> MotionWarpComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* ArrowGroup;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TArray<class UArrowComponent*> Arrows;

public:
	AOnlyUpCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void Action() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Run;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Crouch;


public:
	// 모드 변경
	UFUNCTION(NetMulticast, Reliable)
	void SetModeAndCollision_NMC(EMovementMode InMovementMode, bool InCollision);

	UFUNCTION(Server, Reliable)
	void SetModeAndCollision_Server(EMovementMode InMovementMode, bool InCollision);

	UFUNCTION(BlueprintCallable)
	void SetModeAndCollision(EMovementMode InMovementMode, bool InCollision);

	void Jump() override;
	void Landed(const FHitResult& Hit) override;

	FORCEINLINE int32 GetSpawnIndex() { return SpawnIndex; }

protected:
	UFUNCTION(NetMulticast, Reliable)
	void Walk_NMC();

	UFUNCTION(Server, Reliable)
	void Walk_Server();

	void Walk();

	UFUNCTION(NetMulticast, Reliable)
	void Run_NMC();

	UFUNCTION(Server, Reliable)
	void Run_Server();

	void Run();

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayerMaterialEventOnSpawn();
	void PlayerMaterialEventOnSpawn_Implementation();

public:
	void SetSpawnIndex(int32 InIndex);
	FORCEINLINE void AddSpawnIndex() { SpawnIndex++; }

private:
	int32 SpawnIndex = 1;


};
