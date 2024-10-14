#include "AnimNotifyState/NFS_ActionState.h"
#include "Characters/BombCharacter.h"

FString UNFS_ActionState::GetNotifyName_Implementation() const
{
	return "ActionState";
}

void UNFS_ActionState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    if (MeshComp && MeshComp->GetOwner())
    {
        if (ABombCharacter* Character = Cast<ABombCharacter>(MeshComp->GetOwner()))
        {

        }
    }
}

void UNFS_ActionState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

    if (MeshComp && MeshComp->GetOwner())
    {
        if (ABombCharacter* Character = Cast<ABombCharacter>(MeshComp->GetOwner()))
        {

        }
    }
}
