#pragma once

#include "CoreMinimal.h"
#include "Characters/DefaultCharacter.h"
#include "INHCharacter.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API AINHCharacter : public ADefaultCharacter
{
	GENERATED_BODY()

public:
	AINHCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void Action() override;
	virtual void End_Action() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Run;

	UPROPERTY(BlueprintReadWrite)
	bool isRun;

private:
	UPROPERTY(Replicated)
	float CurrentSpeed;

	UPROPERTY(Replicated)
	bool canRun = false;

public:
	UFUNCTION(Server, Reliable)
	void SetSpeed_Server(bool InCanRun);

	UFUNCTION(BlueprintCallable)
	void SetSpeed(bool InIsRun);
};


