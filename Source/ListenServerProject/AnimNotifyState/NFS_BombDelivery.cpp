#include "AnimNotifyState/NFS_BombDelivery.h"
#include "Global.h"
#include "Characters/BombCharacter.h"

FString UNFS_BombDelivery::GetNotifyName_Implementation() const
{
	return "BombDelivery";
}

void UNFS_BombDelivery::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABombCharacter* BombCharacter = Cast<ABombCharacter>(MeshComp->GetOwner()))
		{
			BombCharacter->HandSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}

	}

}

void UNFS_BombDelivery::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABombCharacter* BombCharacter = Cast<ABombCharacter>(MeshComp->GetOwner()))
		{
			BombCharacter->HandSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}
