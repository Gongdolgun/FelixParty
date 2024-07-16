#include "Components/StateComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

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
		ChangeType(EStateType::Idle);
}

void UStateComponent::SetActionMode()
{
	if (OwnerCharacter->IsLocallyControlled())
		ChangeType(EStateType::Action);
}

void UStateComponent::SetHittedMode()
{
	if (OwnerCharacter->IsLocallyControlled())
		ChangeType(EStateType::Hitted);
}

void UStateComponent::SetDeadMode()
{
	if (OwnerCharacter->IsLocallyControlled())
		ChangeType(EStateType::Dead);
}

EStateType UStateComponent::GetCurrState()
{
	return StateType;
}

void UStateComponent::ChangeType(EStateType InType)
{
	EStateType prevType = StateType;
	StateType = InType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, StateType);
}

