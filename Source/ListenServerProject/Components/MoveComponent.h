#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LISTENSERVERPROJECT_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMoveComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();

	void DoCrouch();
	void Crouch();
	void UnCrouch();

	void EnableControlRotation();
	void DisableControlRotation();

private:
	class ACharacter* Owner;

public:
	bool CanMove = true;

	UPROPERTY(BlueprintReadonly)
	class UOnlineGameInstance* GameInstance;
};
