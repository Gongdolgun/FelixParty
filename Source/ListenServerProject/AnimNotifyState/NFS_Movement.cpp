#include "AnimNotifyState/NFS_Movement.h"
#include "Characters/BombCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

FString UNFS_Movement::GetNotifyName_Implementation() const
{
	return "Movement";
}

void UNFS_Movement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABombCharacter* BombCharacter = Cast<ABombCharacter>(MeshComp->GetOwner()))
		{
			if (!BombCharacter->GetCharacterMovement()->IsFalling())
			{
				BombCharacter->GetCharacterMovement()->DisableMovement();
			}
		}

	}
}

void UNFS_Movement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABombCharacter* BombCharacter = Cast<ABombCharacter>(MeshComp->GetOwner()))
		{
			BombCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}

	}
}
