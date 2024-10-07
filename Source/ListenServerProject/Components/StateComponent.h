#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Misc/Enums.h"
#include "StateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LISTENSERVERPROJECT_API UStateComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UStateComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    FORCEINLINE bool GetAction() { return bInAction; }
    FORCEINLINE bool IsIdleMode() const { return StateType == EStateType::Idle; }
    FORCEINLINE bool IsDeadMode() const { return StateType == EStateType::Dead; }
    FORCEINLINE bool IsActionMode() const { return StateType == EStateType::Action; }

public:
    void BeginAction();
    void EndAction();

    UFUNCTION(BlueprintCallable)
    void SetActionMode();

    void SetIdleMode();
    void SetHittedMode();
    void SetDeadMode();

    UFUNCTION(BlueprintCallable, BlueprintPure)
    EStateType GetCurrState();

public:
    UPROPERTY(Replicated, EditAnywhere, Category = "State")
    EStateType StateType = EStateType::Idle;

    FStateTypeChanged OnStateTypeChanged;

private:
    UFUNCTION(NetMulticast, Reliable)
    void ChangeType_NMC(EStateType InType);

    UFUNCTION(Server, Reliable)
    void ChangeType_Server(EStateType InType);

private:
    class ACharacter* OwnerCharacter;

    bool bInAction = false;

};
