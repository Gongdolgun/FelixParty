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
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, StateType);
}

void UStateComponent::BeginAction()
{
	bInAction = true;
	SetActionMode();

}

void UStateComponent::EndAction()
{
	bInAction = false;

	SetIdleMode();
}

void UStateComponent::SetIdleMode()
{
	if (OwnerCharacter->IsLocallyControlled())
		ChangeType_Server(EStateType::Idle);
}

void UStateComponent::SetActionMode()
{
	if (OwnerCharacter->IsLocallyControlled())
		ChangeType_Server(EStateType::Action);
}

void UStateComponent::SetHittedMode()
{
	if (OwnerCharacter->IsLocallyControlled())
		ChangeType_Server(EStateType::Hitted);
}

void UStateComponent::SetDeadMode()
{
	if (OwnerCharacter->IsLocallyControlled())
		ChangeType_Server(EStateType::Dead);
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
	ChangeType_NMC(InType);
}
