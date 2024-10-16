#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Misc/Enums.h"
#include "TotalScoreCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ATotalScoreCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATotalScoreCharacter();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneCaptureComponent2D* SceneCaptureCamera;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly, Replicated)
	int32 Number;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* OverlayMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTextureRenderTarget2D*> TextureRenderTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> AnimMontages;

private:
	class ADefaultGameState* DefaultGameState;
	UMaterialInstanceDynamic* OverlayMaterialDynamic;

	TArray<UMaterialInstanceDynamic*> MyMaterials;

public:
	UFUNCTION()
	void OnGameStateTypeChanged(EGameStateType InPrevGameType, EGameStateType InNewGameType);

	UFUNCTION(NetMulticast, Reliable)
	void PlayanimMontage_NMC(UAnimMontage* InAnimMontage);

public:
	UFUNCTION(NetMulticast, Reliable)
	void Init();
};
