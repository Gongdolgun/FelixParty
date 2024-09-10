#include "AnimNotifyState/NFS_Collision.h"
#include "Characters/DefaultCharacter.h"

FString UNFS_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UNFS_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ADefaultCharacter* Owner = Cast<ADefaultCharacter>(MeshComp->GetOwner());
	if(Owner != nullptr)
		Owner->OnCollision();
}

void UNFS_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ADefaultCharacter* Owner = Cast<ADefaultCharacter>(MeshComp->GetOwner());
	if (Owner != nullptr)
		Owner->OffCollision();
}