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

	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	UStateComponent* state = Helpers::GetComponent<UStateComponent>(OwnerCharacter);
	if (state == nullptr) return;

	if (OwnerCharacter->IsLocallyControlled())
		state->SetIdleMode();
}

