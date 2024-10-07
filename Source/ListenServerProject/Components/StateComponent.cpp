#include "Components/StateComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UStateComponent::UStateComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

}

void UStateComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter == nullptr) return;
}

void UStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
<<<<<<< HEAD
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, StateType);
=======
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, StateType);
>>>>>>> parent of d6e129d (Merge remote-tracking branch 'origin/JH' into test)
}

void UStateComponent::BeginAction()
{
    bInAction = true;
    SetActionMode();

}

void UStateComponent::EndAction()
{
<<<<<<< HEAD
    bInAction = false;

    SetIdleMode();
=======
	bInAction = false;

	SetIdleMode();
>>>>>>> parent of d6e129d (Merge remote-tracking branch 'origin/JH' into test)
}

void UStateComponent::SetIdleMode()
{
<<<<<<< HEAD
    if (OwnerCharacter->IsLocallyControlled())
        ChangeType_Server(EStateType::Idle);
=======
	if (OwnerCharacter->IsLocallyControlled())
		ChangeType_Server(EStateType::Idle);
>>>>>>> parent of d6e129d (Merge remote-tracking branch 'origin/JH' into test)
}

void UStateComponent::SetActionMode()
{
<<<<<<< HEAD
    if (OwnerCharacter->IsLocallyControlled())
        ChangeType_Server(EStateType::Action);
=======
	if (OwnerCharacter->IsLocallyControlled())
		ChangeType_Server(EStateType::Action);
>>>>>>> parent of d6e129d (Merge remote-tracking branch 'origin/JH' into test)
}

void UStateComponent::SetHittedMode()
{
<<<<<<< HEAD
    if (OwnerCharacter->IsLocallyControlled())
        ChangeType_Server(EStateType::Hitted);
=======
	if (OwnerCharacter->IsLocallyControlled())
		ChangeType_Server(EStateType::Hitted);
>>>>>>> parent of d6e129d (Merge remote-tracking branch 'origin/JH' into test)
}

void UStateComponent::SetDeadMode()
{
<<<<<<< HEAD
    if (OwnerCharacter->IsLocallyControlled())
        ChangeType_Server(EStateType::Dead);
=======
	if (OwnerCharacter->IsLocallyControlled())
		ChangeType_Server(EStateType::Dead);
>>>>>>> parent of d6e129d (Merge remote-tracking branch 'origin/JH' into test)
}

EStateType UStateComponent::GetCurrState()
{
    return StateType;
}

void UStateComponent::ChangeType_NMC_Implementation(EStateType InType)
{
    EStateType prevType = StateType;
    StateType = InType;

    if (OnStateTypeChanged.IsBound())
        OnStateTypeChanged.Broadcast(prevType, StateType);
}

void UStateComponent::ChangeType_Server_Implementation(EStateType InType)
{
<<<<<<< HEAD
    ChangeType_NMC(InType);
=======
	ChangeType_NMC(InType);
>>>>>>> parent of d6e129d (Merge remote-tracking branch 'origin/JH' into test)
}
