#include "AnimNotify/NF_SpawnActor.h"
#include "Global.h"
#include "Characters/BombCharacter.h"

FString UNF_SpawnActor::GetNotifyName_Implementation() const
{
	return "SpawnActor";
}

void UNF_SpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

}
