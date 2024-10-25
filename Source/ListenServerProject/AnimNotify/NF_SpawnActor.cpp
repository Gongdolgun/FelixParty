#include "AnimNotify/NF_SpawnActor.h"
#include "Global.h"
#include "Characters/BombCharacter.h"
#include "Characters/CrashCharacter.h"

FString UNF_SpawnActor::GetNotifyName_Implementation() const
{
	return "SpawnActor";
}

void UNF_SpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp != nullptr)
	{
		ACrashCharacter* Character = Cast<ACrashCharacter>(MeshComp->GetOwner());

		if (Character && Character->IsLocallyControlled())
		{
			Character->SpawnDestoryObject_Server();
		}
	}
}
