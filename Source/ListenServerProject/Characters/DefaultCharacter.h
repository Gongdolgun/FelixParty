#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "Interfaces/IDamage.h"
#include "Misc/Enums.h"
#include "DefaultCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ADefaultCharacter : public ACharacter, public IIDamage
{
	GENERATED_BODY()

public:
	ADefaultCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnCollision();
	virtual void OffCollision();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	virtual void Hit(AActor* InActor, const FHitData& InHitData) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMoveComponent* MoveComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWeaponComponent* WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UChatComponent* ChatComponent;

protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Action;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Option;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Emote;

	UPROPERTY(BlueprintReadOnly)
	TArray<UMaterialInstanceDynamic*> MyMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* OverlayMaterial;

	UPROPERTY(EditAnywhere)
	bool IsMaterialChange = true;

public:
	UFUNCTION(BlueprintCallable)
	virtual void Action();

	UFUNCTION(BlueprintCallable)
	virtual void End_Action();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHP = 100;

	UPROPERTY(BlueprintReadWrite, Replicated)
	float HP;

public:
	UFUNCTION(Server, Reliable)
	void UpdatePlayer_Server();

	UFUNCTION(Server, Reliable)
	void ChangeMaterial_Server(FColor InColor);

	UFUNCTION(NetMulticast, Reliable)
	void ChangeMaterial_NMC(FColor InColor);

	UFUNCTION()
	void PlayMaterialEventOnGameStart(EGameStateType InPrevGameType, EGameStateType InNewGameType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayMaterialEvent();
	void PlayMaterialEvent_Implementation();

	void ChangeMaterial(FColor InColor);

protected:
	void ShowGamePlayOption();

	UFUNCTION(BlueprintCallable)
	void ShowEmoteOption();


public:
	UFUNCTION(NetMulticast, Reliable)
	void PlayEmoteMontage_NMC(UAnimMontage* InAnimMontage);

	UFUNCTION(Server, Reliable)
	void PlayEmoteMontage_Server(UAnimMontage* InAnimMontage);

	UFUNCTION(BlueprintCallable)
	void PlayEmoteMontage(UAnimMontage* InAnimMontage);

private:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> SelectOptionWidget;
	UUserWidget* OptionWidget;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> SelectEmoteWidget;
	UUserWidget* EmoteWidget;
};


