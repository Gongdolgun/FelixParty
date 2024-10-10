#include "AnimNotify/NF_PlaySound3D.h"
#include "GameFramework/Character.h"
#include "Global.h"

FString UNF_PlaySound3D::GetNotifyName_Implementation() const
{
	return "PlaySound3D";
}

void UNF_PlaySound3D::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	Owner = Cast<ACharacter>(MeshComp->GetOwner());
	if (Owner == nullptr)
		return;

	if(Sound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Owner->GetActorLocation());
	}
}
