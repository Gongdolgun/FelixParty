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

public:
	void ChangeMaterial(FColor InColor);

	UFUNCTION(Server, Reliable)
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

	void PlayReadyMontage(class UAnimMontage* InMontage);
};
