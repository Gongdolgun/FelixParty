#include "AnimNotify/NF_SpawnObject.h"
#include "Global.h"
#include "Characters/PushCharacter.h"
#include "GameFramework/Character.h"

FString UNF_SpawnObject::GetNotifyName_Implementation() const
{
	return "SpawnObject";
}

void UNF_SpawnObject::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	APushCharacter* OwerCharacter = Cast<APushCharacter>(MeshComp->GetOwner());
	if (OwerCharacter == nullptr) return;

	if (ActorClass && OwerCharacter->IsLocallyControlled())
	{
		FTransform transform;
		transform.SetLocation(OwerCharacter->GetActorLocation() + 
			(OwerCharacter->GetActorForwardVector() * 60.0f) + OwerCharacter->GetActorRightVector() * LeftDistance);
		transform.SetRotation(FQuat(OwerCharacter->GetActorForwardVector().Rotation()));
		transform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwerCharacter;

		OwerCharacter->SpawnObject_Server(ActorClass, transform);
		//OwerCharacter->GetWorld()->SpawnActor(ActorClass, &transform, SpawnParams);
	}

	
}
