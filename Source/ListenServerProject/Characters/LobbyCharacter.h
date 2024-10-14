#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "Misc/Structures.h"
#include "LobbyCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ALobbyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALobbyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneCaptureComponent2D* SceneCaptureCamera;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTextureRenderTarget2D*> TextureRenderTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* OverlayMaterial;

	UMaterialInstanceDynamic* OverlayMaterialDynamic;

public:
	void ChangeMaterial(FColor InColor);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ChangeMaterial_Server(FColor InColor);

	UFUNCTION(NetMulticast, Reliable)
	void ChangeMaterial_NMC(FColor InColor);

	UFUNCTION(Server, Reliable)
	void UpdatePlayer_Server();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AddSelectedColor(const FString& InColor);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RemoveSelectedColor(const FString& InColor);

	UFUNCTION(NetMulticast, Reliable)
	void PlayReadyMontage_NMC(class UAnimMontage* InMontage);

	UFUNCTION(Server, Reliable)
	void PlayReadyMontage_Server(class UAnimMontage* InMontage);

	UFUNCTION(BlueprintCallable)
	void PlayReadyMontage(class UAnimMontage* InMontage);

	UFUNCTION(Server, Reliable)
	void SetTextureTarget_Server();

	UFUNCTION(NetMulticast, Reliable)
	void SetTextureTarget_NMC(int32 InNumber);
};
