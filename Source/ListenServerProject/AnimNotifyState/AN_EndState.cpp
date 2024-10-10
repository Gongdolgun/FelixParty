#include "AnimNotifyState/AN_EndState.h"
#include "Global.h"
#include "Components/StateComponent.h"

FString UAN_EndState::GetNotifyName_Implementation() const
{
	return "EndAction";
}

void UAN_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	UStateComponent* state = Helpers::GetComponent<UStateComponent>(MeshComp->GetOwner());

	if (state == nullptr) return;

	state->SetIdleMode();
}

